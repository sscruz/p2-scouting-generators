#ifndef gen4_h
#define gen4_h

#include <ap_int.h>

void gen4(bool valid, ap_uint<64> data, ap_uint<64> data_out[4], bool orbit_out[4], bool start_out[4], bool end_out[4], bool valid_out[4], ap_uint<4> CLOCK_RATIO_MINUS_ONE) ;

#endif
