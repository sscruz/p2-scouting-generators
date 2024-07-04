#include "firmware/tkem.h"
#include "firmware/data_tkem.h"
#include "xoshiro_ref.h"
#include <cstdio>
#include <cstdint>
#include<fstream> 


int main(int argc, char **argv) {
  bool ok = true;
  int seed=42;

  Xoshiro256ssRef xoshiro256ref;
  for (unsigned int frame = 0; (frame < 100) && ok; ++frame) {
    //printf("Frame %d\n", frame);
    TkEm outp[12];
    ap_uint<744> random_bits1=xoshiro256ref.get_large_random<744>(frame==0, seed);

    tkem_generator( outp, random_bits1);
    std::ofstream outf;
    std::cout << "Opening " << "event_" << std::to_string(frame) << ".txt" << std::endl;
    outf.open("event_"+std::to_string(frame) + ".txt");
    for (int i=0; i<12;i++){
      outf << outp[i].hwPt << " "
	   << int(outp[i].hwEta) << " "
	   << int(outp[i].hwPhi) << " "
	   << int(outp[i].hwTrkIsol) << std::endl;
    }
    outf.close();
  }
  printf("Done \n");
  return 0;
}
