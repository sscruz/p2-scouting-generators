#ifndef ALGO_DATA_H
#define ALGO_DATA_H

#include "ap_int.h"

#define n_pu_jets 30
#define pu_jet_depth 1
#define pu_jet_depth2 2
#define pu_jet_npart 4

#define nbits_pt 14
#define nbits_etaphi 10

#define pu_jet_pt_min 2.0
#define pu_jet_pt_decay 5.0


typedef ap_uint<nbits_pt>  pt_t;       // 1 unit = 0.25 GeV;
typedef ap_uint<nbits_pt>  x_t ;      // step 6.103515625e-05, maximum 0.99993896484375
typedef ap_int<nbits_etaphi>  etaphi_t;   // 1 unit = 0.01;     max = 5.12

struct Particle {
  pt_t hwPt=0;
  etaphi_t hwEta=0;
  etaphi_t hwPhi=0;
  bool     hwPartType=0; // 1 gluon, 0 quark
  bool     hwIsStable=0;
};



#endif
