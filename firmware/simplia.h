#ifndef simplia_h
#define simplia_h

#include "data.h"

#define random_bits_per_splitting 19
#define max_depth 5
#define max_depth2 32
#define lut_size 9

void shower(const Particle theparticle[1], Particle outparticles[max_depth2],  ap_uint<random_bits_per_splitting*(max_depth2-1)> rand);

#endif 
