#include "firmware/simplia.h"
#include "firmware/data.h"
#include <cstdio>
#include <cstdint>

// From https://en.wikipedia.org/wiki/Xorshift#xoshiro256**
// Adapted from https://prng.di.unimi.it/xoshiro256starstar.c
// David Blackman and Sebastiano Vigna (vigna@acm.org)

int main(int argc, char **argv) {
  bool ok = true;
  int seed=42;

  for (unsigned int frame = 0; (frame < 200) && ok; ++frame) {
    Particle inp[1]; inp[0].hwPt=50*4;
    Particle outp[32];
    shower( inp, outp );
    printf("\n\n");
    printf("We have one event\n");
    for (int i=0; i < 32; ++i){
      printf("   Particle with hwPt=%d\n", outp[i].hwPt);
    }

  }
  printf("\n");

}
