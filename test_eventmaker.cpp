#include "firmware/data.h"
#include "firmware/eventmaker.h"
#include "xoshiro_ref.h"
#include "pattern_serializer.h"

#include <cstdio>
#include <cstdint>


const int n_rand_per_jet=8*pu_jet_npart+pu_jet_npart*10+random_bits_per_splitting*(pu_jet_depth2*2-1)+(lut_size+20);

int main(int argc, char **argv) {

  bool ok = true;
  int seed=42;
  Xoshiro256ssRef xoshiro256ref;

  const unsigned int multiplicity=50;
  HumanReadablePatternSerializer dumper("output.dump");

  for (unsigned int nburn = 0; nburn < 5000; ++nburn){
    xoshiro256ref.getrandom(nburn==0,seed);
  }

  for (unsigned int frame = 0; (frame < 20) && ok; ++frame) {

    PackedPuppiObj out_particles[n_pu_jets*pu_jet_npart];

    ap_uint<10> vertex_rand = xoshiro256ref.getrandom(false,seed) & 1023;
    ap_uint<n_rand_per_jet>  bigrand[n_pu_jets];
    for (int i=0; i < n_pu_jets; ++i){
      bigrand[i]=xoshiro256ref.get_large_random<n_rand_per_jet>(false, seed);
    }

    bitpattern_pileup(out_particles, vertex_rand, bigrand);


    // now we are gonna store everything 
    l1ct::PuppiObj simplia_particles[n_pu_jets*pu_jet_npart];
    int npuppi=0;
    for(int i=0;i<n_pu_jets*pu_jet_npart;++i){
      simplia_particles[i] = l1ct::PuppiObj::unpack(out_particles[i]);
    }
    dumper.dump_puppi(n_pu_jets*pu_jet_npart, "ref", simplia_particles);

  }

}

