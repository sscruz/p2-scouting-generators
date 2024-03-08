#ifndef ALGO_DATA_H
#define ALGO_DATA_H

#include "ap_int.h"

typedef ap_uint<14>  pt_t;       // 1 unit = 0.25 GeV;
typedef ap_uint<14>  x_t ;      // step 6.103515625e-05, maximum 0.99993896484375
typedef ap_int<10>  etaphi_t;   // 1 unit = 0.01;     max = 5.12

struct Particle {
  pt_t hwPt=0;
  etaphi_t hwEta=0;
  etaphi_t hwPhi=0;
  bool     hwPartType=0; // 1 gluon, 0 quark
  bool     hwIsStable=0;
};

#endif
