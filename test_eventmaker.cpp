#include "firmware/data.h"
#include "firmware/eventmaker.h"
#include "xoshiro_ref.h"
#include "pattern_serializer.h"

#include <cstdio>
#include <cstdint>


const int nbigrand=10+n_pu_jets*pu_jet_npart*8+n_pu_jets*pu_jet_npart*10+random_bits_per_splitting*(pu_jet_depth2*2-1)*n_pu_jets+n_pu_jets*(lut_size+20);

int main(int argc, char **argv) {

  bool ok = true;
  int seed=42;
  Xoshiro256ssRef xoshiro256ref;

  const unsigned int multiplicity=50;
  HumanReadablePatternSerializer dumper("output.dump");

  for (unsigned int frame = 0; (frame < 50) && ok; ++frame) {

    PackedPuppiObj out_particles[8*16];

    ap_uint<10> vertex_rand = xoshiro256ref.getrandom(frame==0,seed) & 1023;
    ap_uint<nbigrand>  bigrand=xoshiro256ref.get_large_random<nbigrand>(false, seed);

    bitpattern_pileup(out_particles, bigrand);

    // now we are gonna store everything 
    l1ct::PuppiObj simplia_particles[n_pu_jets*pu_jet_npart];
    for(int i=0;i<n_pu_jets*pu_jet_npart;++i) simplia_particles[i] = l1ct::PuppiObj::unpack(out_particles[i]);
    dumper.dump_puppi(n_pu_jets*pu_jet_npart, "ref", simplia_particles);

  }
  printf("\n");

}

