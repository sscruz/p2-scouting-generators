#include "firmware/simplia.h"
#include "firmware/data.h"
#include "xoshiro_ref.h"

#include <cstdio>
#include <cstdint>



int main(int argc, char **argv) {
  bool ok = true;
  int seed=42;
  Xoshiro256ssRef xoshiro256ref;

  for (unsigned int frame = 0; (frame < 200) && ok; ++frame) {
    Particle inp; inp.hwPt=50*4;
    Particle outp[32];
    ap_uint<569> rand=xoshiro256ref.get_large_random<569>(frame == 0, seed);
    shower( inp, outp, rand );
    printf("\n\n");
    printf("We have one event\n");
    for (int i=0; i < 32; ++i){
      printf("   Particle with hwPt=%d\n", outp[i].hwPt);
    }

  }
  printf("\n");

}
