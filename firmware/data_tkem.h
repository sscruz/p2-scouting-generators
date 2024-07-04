#ifndef TKEM_DATA_H
#define TKEM_DATA_H

#include "ap_int.h"

#define nbits_pt 14
#define nbits_etaphi 10
#define nbits_pteg 9
#define nbits_isoeg 10

#define n_eg 12
#define n_egE 4
#define n_eg_all 16

typedef ap_uint<nbits_pt>  pt_t;       // 1 unit = 0.25 GeV;
typedef ap_uint<nbits_pteg>  pteg_t;       // 1 unit = 0.25 GeV; max 128 GeV
typedef ap_int<nbits_etaphi>  etaphi_t;   // 1 unit = 0.049;     max = 2.5
typedef ap_uint<2>  ne_t;
typedef ap_uint<nbits_isoeg>  iso_t; // 1 unit = 0.01; max = 10.24

struct TkEm {
  pt_t hwPt=0;
  etaphi_t hwEta=0;
  etaphi_t hwPhi=0;
  iso_t hwTrkIsol=0;
};




#endif
