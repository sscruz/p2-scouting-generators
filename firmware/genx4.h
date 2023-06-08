#ifndef genx4_h
#define genx4_h

#include <ap_int.h>

void genx4(bool valid, ap_uint<32> orbit, ap_uint<64> data, ap_uint<64> data_out[4], bool orbit_out[4], bool start_out[4], bool end_out[4], bool valid_out[4]) ;

#endif
