#ifndef ALGO_DATA_H
#define ALGO_DATA_H
#define AP_INT_MAX_W 2000

#include "ap_int.h"
#include "fromcmssw.h" 


#define n_pu_jets 30
#define pu_jet_depth 1
#define pu_jet_depth2 2
#define pu_jet_npart 4

#define nbits_pt 14
#define nbits_etaphi 10
#define nbits_zt 10

#define pu_jet_pt_min 2.0
#define pu_jet_pt_decay 5.0
#define random_bits_per_splitting 19
#define lut_size 9

typedef ap_uint<64> PackedPuppiObj;

template<unsigned int nbits>
void print_ap( ap_uint<nbits> bits)
{
#ifndef __SYNTHESIS__
  for (unsigned int i=0; i<nbits; ++i){
    printf("%d",int(bits.bit(i)));
  }
  printf("\n");
#endif
}


typedef ap_uint<nbits_pt>  pt_t;       // 1 unit = 0.25 GeV;
typedef ap_uint<nbits_pt>  x_t ;      // step 6.103515625e-05, maximum 0.99993896484375
typedef ap_int<nbits_etaphi>  etaphi_t;   // 1 unit = 0.01;     max = 5.12
typedef ap_int<nbits_zt> z0_t; // 25.6 cm / 1 unit= 0.05 cm

struct Parton {
  pt_t hwPt=0;
  etaphi_t hwEta=0;
  etaphi_t hwPhi=0;
  bool     hwPartType=0; // 1 gluon, 0 quark
  bool     hwIsStable=0;

  inline bool operator>(const Parton &other) const { return hwPt > other.hwPt; }
  inline bool operator<(const Parton &other) const { return hwPt < other.hwPt; }


};


#endif
