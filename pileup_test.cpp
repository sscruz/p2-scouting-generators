#include "firmware/pileup.h"
#include "firmware/data.h"
#include <cstdio>
#include <cstdint>
#include<fstream> 


int main(int argc, char **argv) {
  bool ok = true;
  int seed=42;

  for (unsigned int frame = 0; (frame < 100000) && ok; ++frame) {
    //printf("Frame %d\n", frame);
    Particle outp[120];
    pileup_generator( outp );
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
