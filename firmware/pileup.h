#ifndef pileup_h
#define pileup_h
#include"simplia.h"


void pileup_generator(Parton out_particles[n_pu_jets*pu_jet_npart], ap_uint<random_bits_per_splitting*(pu_jet_depth2*2-1)> shower_rand[n_pu_jets], ap_uint<(lut_size+20)> jetrand[n_pu_jets]);

#endif
