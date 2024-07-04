#ifndef tkem_h
#define tkem_h
#include "data_tkem.h"

#define lut_size_eg 10


void tkem_generator(TkEm out_egEs[n_eg], ap_uint<n_eg_all*(lut_size_eg+10+10+4+1+10)+n_egE*6> rand1);

#endif
