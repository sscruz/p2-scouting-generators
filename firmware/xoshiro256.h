#ifndef xoshiro256_h
#define xoshiro256_h

#include <ap_int.h>
#define ninstances 40 // careful, need to edit "seeds" and "isfirst" below 


	     


template<int k>
inline ap_uint<64> rol(ap_uint<64> x) {
  ap_uint<64> ret;
  ret(63, k)  = x(63-k, 0);
  ret(k-1, 0) = x(63, 63-k+1);
  return ret;
}

template<int instance>
ap_uint<64> xoshiro256hw() {
#pragma HLS pipeline II = 1
  static ap_uint<64> state[ninstances][4] = { 0, 0, 0, 0 };
  static bool isfirst[ninstances]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}; // :)
  static ap_uint<64> seeds[ninstances] = { // from chatgpt
					  0x8c76a8bd57e42d6d,
					  0x9a69a21a155d0a82,
					  0x84b6e90f390c5eeb,
					  0x75981b1f76b72b7b,
					  0xa497c96a754b249e,
					  0xd3b3816aee5289e2,
					  0x6093459465f6176e,
					  0x476c14f284c8d29c,
					  0x19e9e1c8f137c09f,
					  0x63b11d68b8e4850a,
					  0x9838162c98d5c0f8,
					  0xa784de4bb75f731d,
					  0x1b3b5d7e1bc89a2b,
					  0xf6a15cfac814a0b9,
					  0x41c8491272387589,
					  0xf72697c7ecb1ef3b,
					  0x90c7dc31464a38f8,
					  0x34fc2a3a964f6c1b,
					  0x5b400dcf71a2be58,
					  0x4e9bc7a3aeb238b1,
					  0x918b1b12f9b64f29,
					  0xe9e2df91db9a4986,
					  0xf8f5cfa17612a3db,
					  0x67b65a19c94d3309,
					  0x5f78b4a76a005b39,
					  0x2c11630e185f2ae4,
					  0x5e44f5ad21d8b7b8,
					  0xda4d92e364b8c586,
					  0x33b258d7088b5a91,
					  0x143c0fa8576c0fb4,
					  0x40fa7ee0a1b917ad,
					  0x4fd7d14c70112961,
					  0x3705311d2eb38c1e,
					  0xa2328cf1aefbf540,
					  0xa53c5201bf7d1897,
					  0x24b037a6e44a2bfc,
					  0x8261f2344dc6cf2f,
					  0x2f9a0059c2720e68,
					  0xaddbd28b92687239,
					  0x53ad5e66ebe93668,
  };

#pragma HLS array_partition variable = state complete
#pragma HLS array_partition variable = isfirst complete
  ap_uint<64> step0 = (state[instance][1] << 2)+state[instance][1]; // i.e. *5
  ap_uint<64> step1 = rol<7>(step0);
  ap_uint<64> step2 = step1 + (step1 << 3); // i.e. * 9
  if (isfirst[instance]) {
    state[instance][0] = seeds[instance] ^ 0x6b54c6575a2c794b;
    state[instance][1] = rol<13>(seeds[instance]) ^ 0x514e9359bb2950cb;
    state[instance][2] = rol<27>(seeds[instance]) ^ 0xc62c17550e3d1904;
    state[instance][3] = rol<42>(seeds[instance]) ^ 0xf47e648fdd3da813;
    isfirst[instance]=0;
  } else {
    ap_uint<64> t = state[instance][1] << 17;
    state[instance][2] ^= state[instance][0];
    state[instance][3] ^= state[instance][1];
    state[instance][1] ^= state[instance][2];
    state[instance][0] ^= state[instance][3];
    state[instance][2] ^= t;
    state[instance][3] = rol<45>(state[instance][3]);
  }
  return step2;
}



namespace generator{
  template<int iteration, int instances, int randombits, int iteration_offset>
  struct helper{
    static ap_uint<randombits> get_large_random(){
      ap_uint<randombits> chunk_rand=xoshiro256hw<(iteration%instances)+iteration_offset>();
      ap_uint<randombits> previous=helper<iteration-1,instances,randombits,iteration_offset>::get_large_random();
      ap_uint<randombits> ret=((chunk_rand) << ((iteration-1)*64)) + previous;
      return ret;
    }
  };


  template<int instances, int randombits, int iteration_offset>
  struct helper<1,instances,randombits,iteration_offset>{
    static ap_uint<randombits> get_large_random(){
      auto chunk_rand=xoshiro256hw<iteration_offset>();
      return chunk_rand;
    }
  };
}


#endif
