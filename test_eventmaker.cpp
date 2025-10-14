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

  const unsigned int multiplicity=50;
  HumanReadablePatternSerializer dumper("output.dump");

  bool setseed=true;
  for (unsigned int frame = 0; (frame < 50) && ok; ++frame) {
    printf("New event\n");
    PackedPuppiObj out_particles[80*pu_jet_npart];
    for (int ijet = 0;ijet<80;++ijet){
      generate_one_pu_jet( out_particles + pu_jet_npart*ijet, seed, setseed);
      setseed=false;
    }

    // now we are gonna store everything 
    l1ct::PuppiObj simplia_particles[80*pu_jet_npart];
    int npuppi=0;
    for(int i=0;i<80*pu_jet_npart;++i){
      simplia_particles[i] = l1ct::PuppiObj::unpack(out_particles[i]);
    }
    dumper.dump_puppi(80*pu_jet_npart, "ref", simplia_particles);

  }

}

