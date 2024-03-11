#include "firmware/simplia.h"
#include "firmware/data.h"
#include <cstdio>
#include <cstdint>

// From https://en.wikipedia.org/wiki/Xorshift#xoshiro256**
// Adapted from https://prng.di.unimi.it/xoshiro256starstar.c
// David Blackman and Sebastiano Vigna (vigna@acm.org)
class Xoshiro256ssRef {
  public:

    Xoshiro256ssRef() {}

    uint64_t rol64(uint64_t x, int k) { return (x << k) | (x >> (64 - k)); }

    uint64_t operator()(bool setseed, uint64_t seed) {
      uint64_t const result = rol64(state[1] * 5, 7) * 9;
      if (setseed) {
        state[0] = seed ^ 0x6b54c6575a2c794b;
        state[1] = rol64(seed, 13) ^ 0x514e9359bb2950cb;
        state[2] = rol64(seed, 27) ^ 0xc62c17550e3d1904;
        state[3] = rol64(seed, 42) ^ 0xf47e648fdd3da813;
      } else {
        uint64_t const t = state[1] << 17;
        state[2] ^= state[0];
        state[3] ^= state[1];
        state[1] ^= state[2];
        state[0] ^= state[3];
        state[2] ^= t;
        state[3] = rol64(state[3], 45);
      }
      return result;
    }

  private:
    uint64_t state[4] = { 0, 0, 0, 0 };
};

int main(int argc, char **argv) {
  bool ok = true;
  int seed=42;
  Xoshiro256ssRef xoshiro256ref;
  uint64_t data_ref = xoshiro256ref(true, seed);

  for (unsigned int frame = 0; (frame < 2) && ok; ++frame) {
    Particle inp[1]; inp[0].hwPt=50*4;
    Particle outp[32];
    shower( inp, outp, frame==0);
    printf("We have one event\n");
    for (int i=0; i < 32; ++i){
      printf("   Particle with hwPt=%d\n", outp[i].hwPt);
    }

  }
  printf("\n");

}
