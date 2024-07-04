#include "tkem.h"
#include <cmath>
#include "luts_tkem.h"

void bubble_sort(TkEm array[n_eg_all]) {
    for (int i = 0; i < n_eg_all - 1; i++) {
#pragma HLS unroll
        for (int j = 0; j < n_eg_all - i - 1; j++) {
#pragma HLS unroll
            if (array[j].hwPt < array[j + 1].hwPt) {
                // Swap elements
                TkEm temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

void get_tkem( TkEm tkem[n_eg], ap_uint<n_eg_all*(lut_size_eg+10+10+4+1+10)+n_egE*6> rand_egall){


  TkEm tkem16[n_eg_all]; // Generate 16 TkEm objects: 12 in the barrel and 4 in the endcaps. Use random numbers to choose whether endcap objects have non-zero pt, order all 16 objects, and output the 12 leading ones

  ap_uint<n_eg_all*lut_size_eg> rand_pt = rand_egall(n_eg_all*lut_size_eg-1,0);   // 5*64 bits, using 2 instances of the generator
  ap_uint<n_eg_all*10>          rand_eta = rand_egall(n_eg_all*(10+lut_size_eg)-1,n_eg_all*lut_size_eg); 
  ap_uint<n_eg_all*10>          rand_phi = rand_egall(n_eg_all*(lut_size_eg+10+10)-1,n_eg_all*(lut_size_eg+10));
  ap_uint<n_eg_all*4>           rand_lutchoice = rand_egall(n_eg_all*(lut_size_eg+10+10+4)-1,n_eg_all*(lut_size_eg+10+10));  // 1 out of 16 (4 bits) values points to high LUT
  ap_uint<n_eg_all*1>           rand_signeta = rand_egall(n_eg_all*(lut_size_eg+10+10+4+1)-1,n_eg_all*(lut_size_eg+10+10+4));
  ap_uint<n_eg_all*10>          rand_iso = rand_egall(n_eg_all*(lut_size_eg+10+10+4+1+10)-1,n_eg_all*(lut_size_eg+10+10+4+1));
  ap_uint<n_egE*6>              rand_probendcap = rand_egall(n_eg_all*(lut_size_eg+10+10+4+1+10)+(n_egE*6)-1,n_eg_all*(lut_size_eg+10+10+4+1+10));  // 3/64 chance for endcap TkEm to have nonzero value

  // Start with endcap TkEm
  for (int iegE=0; iegE<n_egE; ++iegE){
#pragma HLS unroll
    TkEm p;
    if (int(rand_probendcap(6*(iegE+1)-1,6*iegE))>2){ // in 61/64 cases, set the endcap TkEm pt to 0 (dont generate endcap TkEm)
       p.hwPt=0;
    }
    else{
       if (int(rand_lutchoice(4*(iegE+1)-1,4*iegE))==1){ // in 1 out of 16 cases use LUT with high pt values
          p.hwPt=table_highE_tkem[ int(rand_pt(lut_size_eg*(iegE+1)-1,lut_size_eg*iegE))];
       }
       else{ 
          p.hwPt=table_lowE_tkem[ int(rand_pt(lut_size_eg*(iegE+1)-1,lut_size_eg*iegE))];
       }
       if (int(rand_lutchoice(1*(iegE+1)-1,1*iegE))==1){ // positive eta
          p.hwEta=table_etaE_tkem[ int(rand_eta(lut_size_eg*(iegE+1)-1,lut_size_eg*iegE))];
       }
       else{
          p.hwEta=-table_etaE_tkem[ int(rand_eta(lut_size_eg*(iegE+1)-1,lut_size_eg*iegE))];
       }
       //p.hwEta=0.4*(rand_eta(nbits_etaphi*(iegE+1)-1,nbits_etaphi*iegE)-(1 << (nbits_etaphi-3)))+307;
       p.hwPhi=rand_phi(nbits_etaphi*(iegE+1)-1,nbits_etaphi*iegE)-(1 << (nbits_etaphi-2));
       if (p.hwPt<40){
	  p.hwTrkIsol=table_isolt10E_tkem[ int(rand_iso(lut_size_eg*(iegE+1)-1,lut_size_eg*iegE))];
       }
       else{
	  p.hwTrkIsol=table_isogt10E_tkem[ int(rand_iso(lut_size_eg*(iegE+1)-1,lut_size_eg*iegE))];
       }
    }
    tkem16[iegE]=p;
  }

  // Now generate barrel TkEm
  for (int iegB=n_egE; iegB<n_eg_all; ++iegB){
#pragma HLS unroll
    TkEm p;
    if (int(rand_lutchoice(4*(iegB+1)-1,4*iegB))==1){ // in 1 out of 16 cases use LUT with high pt values
       p.hwPt=table_highB_tkem[ int(rand_pt(lut_size_eg*(iegB+1)-1,lut_size_eg*iegB))];
    }
    else{
       p.hwPt=table_lowB_tkem[ int(rand_pt(lut_size_eg*(iegB+1)-1,lut_size_eg*iegB))];
    }
    //p.hwEta=0.6*(rand_eta(nbits_etaphi*(iegB+1)-1,nbits_etaphi*iegB)-(1 << (nbits_etaphi-1)));
    if (int(rand_lutchoice(1*(iegB+1)-1,1*iegB))==1){ // positive eta
       p.hwEta=table_etaB_tkem[ int(rand_eta(lut_size_eg*(iegB+1)-1,lut_size_eg*iegB))];
    }
    else{   
       p.hwEta=-table_etaB_tkem[ int(rand_eta(lut_size_eg*(iegB+1)-1,lut_size_eg*iegB))];
    }
    p.hwPhi=rand_phi(nbits_etaphi*(iegB+1)-1,nbits_etaphi*iegB)-(1 << (nbits_etaphi-2));

    // Based on pt choose with LUT to read to access the isolation
    if (p.hwPt<20){
       p.hwTrkIsol=table_isolt5B_tkem[ int(rand_iso(lut_size_eg*(iegB+1)-1,lut_size_eg*iegB))];
    }
    else if (p.hwPt<40){
       p.hwTrkIsol=table_iso5to10B_tkem[ int(rand_iso(lut_size_eg*(iegB+1)-1,lut_size_eg*iegB))];
    }
    else{
       p.hwTrkIsol=table_isogt10B_tkem[ int(rand_iso(lut_size_eg*(iegB+1)-1,lut_size_eg*iegB))];
    }     

    tkem16[iegB]=p;
  }

  // Sort 16 elements and fill first 12 TkEm objects from both barrel and endcaps
  bubble_sort(tkem16);

  for (int ieg=0; ieg<n_eg; ++ieg){
#pragma HLS unroll
    tkem[ieg]=tkem16[ieg];
  }


}

void tkem_generator(TkEm out_egEs[n_eg], ap_uint<n_eg_all*(lut_size_eg+10+10+4+1+10)+n_egE*6> rand1)
{
#pragma HLS pipeline II=8
#pragma HLS array_partition variable=out_egEs complete 
  get_tkem( out_egEs, rand1 );
}
