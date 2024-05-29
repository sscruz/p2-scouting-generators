#include "firmware/simplia.h"
#include "firmware/data.h"
#include <cstdio>
#include <cstdint>



int main(int argc, char **argv) {
  bool ok = true;
  int seed=42;
  Xoshiro256ssRef xoshiro256ref;

  for (unsigned int frame = 0; (frame < 200) && ok; ++frame) {
    Particle inp; inp.hwPt=50*4;
    Particle outp[32];
    ap_uint<569> rand=xoshiro256ref(frame == 0, seed);
    for (int i=0; i<8; ++i){
      rand = (rand << 64) + xoshiro256ref(false, seed);
    }
    rand = (rand << 13) + xoshiro256ref(false, seed) & ((1<<13)-1);
    // print_ap(rand);
    shower( inp, outp, rand );
    printf("\n\n");
    printf("We have one event\n");
    for (int i=0; i < 32; ++i){
      printf("   Particle with hwPt=%d\n", outp[i].hwPt);
    }

  }
  printf("\n");

}
