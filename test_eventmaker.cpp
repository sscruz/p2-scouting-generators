#include "firmware/data.h"
#include "firmware/eventmaker.h"
#include "xoshiro_ref.h"
#include "pattern_serializer.h"

#include <cstdio>
#include <cstdint>



int main(int argc, char **argv) {

  bool ok = true;
  int seed=42;
  Xoshiro256ssRef xoshiro256ref;

  const unsigned int multiplicity=50;
  for (unsigned int frame = 0; (frame < 100) && ok; ++frame) {

    PackedPuppiObj out_particles[n_pu_jets*pu_jet_npart];

    ap_uint<10> vertex_rand = xoshiro256ref.getrandom(frame==0,seed) & 1023;
    ap_uint<n_pu_jets*pu_jet_npart*8>  vz_resolution_rand=xoshiro256ref.get_large_random<n_pu_jets*pu_jet_npart*8>(false, seed);
    ap_uint<n_pu_jets*pu_jet_npart*10> hadronization_rand=xoshiro256ref.get_large_random<n_pu_jets*pu_jet_npart*10>(false, seed);
    ap_uint<random_bits_per_splitting*(pu_jet_depth2*2-1)*n_pu_jets> shower_rand=xoshiro256ref.get_large_random<random_bits_per_splitting*(pu_jet_depth2*2-1)*n_pu_jets>(false, seed, true);
    ap_uint<n_pu_jets*(lut_size+20)> jetrand = xoshiro256ref.get_large_random<n_pu_jets*(lut_size+20)>(false, seed);

    bitpattern_pileup(out_particles, 
                       vertex_rand, vz_resolution_rand , hadronization_rand, shower_rand, jetrand);

    // now we are gonna store everything 
    l1ct::PuppiObj simplia_particles[n_pu_jets*pu_jet_npart];
    for(int i=0;i<n_pu_jets*pu_jet_npart;++i) simplia_particles[i] = l1ct::PuppiObj::unpack(out_particles[i]);
    HumanReadablePatternSerializer dumper("-");
    dumper.dump_puppi(n_pu_jets*pu_jet_npart, "ref", simplia_particles);

  }
  printf("\n");

}

