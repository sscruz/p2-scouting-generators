#include "firmware/data.h"

class Xoshiro256ssRef {
  public:

    Xoshiro256ssRef() {}

    uint64_t rol64(uint64_t x, int k) { return (x << k) | (x >> (64 - k)); }

    uint64_t getrandom(bool setseed, uint64_t seed) {
      uint64_t const result = rol64(state[1] * 5, 7) * 9;
      if (setseed) {
        state[0] = seed ^ 0x6b54c6575a2c794b;
        state[1] = rol64(seed, 13) ^ 0x514e9359bb2950cb;
        state[2] = rol64(seed, 27) ^ 0xc62c17550e3d1904;
        state[3] = rol64(seed, 42) ^ 0xf47e648fdd3da813;
      } else {
        uint64_t const t = state[1] << 17;
        state[2] ^= state[0];
        state[3] ^= state[1];
        state[1] ^= state[2];
        state[0] ^= state[3];
        state[2] ^= t;
        state[3] = rol64(state[3], 45);
      }
      return result;
    }

    template< unsigned int nbits>
    ap_uint<nbits> get_large_random(bool setseed, uint64_t seed, bool debug=false){
        ap_uint<nbits> ret = getrandom(setseed, seed);

        for (unsigned int i=0; i< (nbits/64) -1; ++i ){
          ret = (ret << 64) + getrandom(false, seed);
        }
        ap_uint<nbits%64> mask=(1L << (nbits%64))-1;
                                                         
        ret = (ret << (nbits%64)) + (getrandom(false,seed) & mask)  ;
          
        return ret;
    }

  private:
    uint64_t state[4] = { 0, 0, 0, 0 };
};
