#ifndef xoshiro256_h
#define xoshiro256_h

#include <ap_int.h>

ap_uint<64> xoshiro256hw(bool setseed, ap_uint<64> seed);
ap_uint<64> xoshiro256hw2(bool setseed, ap_uint<64> seed);
ap_uint<64> xoshiro256hw3(bool setseed, ap_uint<64> seed);

#endif
