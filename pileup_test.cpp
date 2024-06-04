#include "firmware/pileup.h"
#include "firmware/data.h"
#include "xoshiro_ref.h"
#include <cstdio>
#include <cstdint>
#include<fstream> 


int main(int argc, char **argv) {
  bool ok = true;
  int seed=42;
  
  Xoshiro256ssRef xoshiro256ref;
  for (unsigned int frame = 0; (frame < 100000) && ok; ++frame) {
    //printf("Frame %d\n", frame);
    Parton outp[120];
    ap_uint<1710> random_bits1=xoshiro256ref.get_large_random<1710>(frame==0, seed); 
    ap_uint<870> random_bits2=xoshiro256ref.get_large_random<870>(false, seed); 

    pileup_generator( outp, random_bits1, random_bits2 );
    std::ofstream outf;
    std::cout << "Opening " << "event_" << std::to_string(frame) << ".txt" << std::endl;
    outf.open("event_"+std::to_string(frame) + ".txt");
    for (int i=0; i<120;i++){
      outf << outp[i].hwPt << " "
	   << int(outp[i].hwEta) << " "
	   << int(outp[i].hwPhi) << std::endl;
    }
    outf.close();
  }
  printf("Done \n");
  return 0;
}
