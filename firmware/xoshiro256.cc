#include "xoshiro256.h"
#include <cassert>


template<int k>
inline ap_uint<64> rol(ap_uint<64> x) {
  ap_uint<64> ret;
  ret(63, k)  = x(63-k, 0);
  ret(k-1, 0) = x(63, 63-k+1);
  return ret;
}

ap_uint<64> xoshiro256hw(bool setseed, ap_uint<64> seed) {
  #pragma HLS pipeline II = 1
  static ap_uint<64> state[4] = { 0, 0, 0, 0 };
  #pragma HLS array_partition variable = state complete
  ap_uint<64> step0 = (state[1] << 2)+state[1]; // i.e. *5
  ap_uint<64> step1 = rol<7>(step0);
  ap_uint<64> step2 = step1 + (step1 << 3); // i.e. * 9
  if (setseed) {
    state[0] = seed ^ 0x6b54c6575a2c794b;
    state[1] = rol<13>(seed) ^ 0x514e9359bb2950cb;
    state[2] = rol<27>(seed) ^ 0xc62c17550e3d1904;
    state[3] = rol<42>(seed) ^ 0xf47e648fdd3da813;
  } else {
    ap_uint<64> t = state[1] << 17;
    state[2] ^= state[0];
    state[3] ^= state[1];
    state[1] ^= state[2];
    state[0] ^= state[3];
    state[2] ^= t;
    state[3] = rol<45>(state[3]);
  }
  return step2;
}

ap_uint<64> xoshiro256hw2(bool setseed, ap_uint<64> seed) {
  #pragma HLS pipeline II = 1
  static ap_uint<64> state2[4] = { 0, 0, 0, 0 };
  #pragma HLS array_partition variable = state2 complete
  ap_uint<64> step0 = (state2[1] << 2)+state2[1]; // i.e. *5
  ap_uint<64> step1 = rol<7>(step0);
  ap_uint<64> step2 = step1 + (step1 << 3); // i.e. * 9
  if (setseed) {
    state2[0] = seed ^ 0x6b54c6575a2c794b;
    state2[1] = rol<13>(seed) ^ 0x514e9359bb2950cb;
    state2[2] = rol<27>(seed) ^ 0xc62c17550e3d1904;
    state2[3] = rol<42>(seed) ^ 0xf47e648fdd3da813;
  } else {
    ap_uint<64> t = state2[1] << 17;
    state2[2] ^= state2[0];
    state2[3] ^= state2[1];
    state2[1] ^= state2[2];
    state2[0] ^= state2[3];
    state2[2] ^= t;
    state2[3] = rol<45>(state2[3]);
  }
  return step2;
}

ap_uint<64> xoshiro256hw3(bool setseed, ap_uint<64> seed) {
  #pragma HLS pipeline II = 1
  static ap_uint<64> state3[4] = { 0, 0, 0, 0 };
  #pragma HLS array_partition variable = state3 complete
  ap_uint<64> step0 = (state3[1] << 2)+state3[1]; // i.e. *5
  ap_uint<64> step1 = rol<7>(step0);
  ap_uint<64> step2 = step1 + (step1 << 3); // i.e. * 9
  if (setseed) {
    state3[0] = seed ^ 0x6b54c6575a2c794b;
    state3[1] = rol<13>(seed) ^ 0x514e9359bb2950cb;
    state3[2] = rol<27>(seed) ^ 0xc62c17550e3d1904;
    state3[3] = rol<42>(seed) ^ 0xf47e648fdd3da813;
  } else {
    ap_uint<64> t = state3[1] << 17;
    state3[2] ^= state3[0];
    state3[3] ^= state3[1];
    state3[1] ^= state3[2];
    state3[0] ^= state3[3];
    state3[2] ^= t;
    state3[3] = rol<45>(state3[3]);
  }
  return step2;
}
