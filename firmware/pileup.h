#ifndef pileup_h
#define pileup_h
#include"simplia.h"


void pileup_generator(Particle out_particles[n_pu_jets*pu_jet_npart], ap_uint<random_bits_per_splitting*(pu_jet_depth2*2-1)*n_pu_jets>, ap_uint<n_pu_jets*(lut_size+20)>);

#endif
