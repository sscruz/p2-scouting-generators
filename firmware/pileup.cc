#include "pileup.h"
#include <cmath>

// rng breakdown
// 0-1: pu jets, pt
// 2-3: pu jets, eta
// 4-5: pu jets, phi


void _lut_pt_exp_init( pt_t pt_exp_lut[512] ){

  for (int i=0; i<512;++i){
    pt_exp_lut[i]=((pu_jet_pt_min-pu_jet_pt_decay*log(1-float(i)/512.)))*4; // pt_t is in units of 0.25, so multiply by 4. with lut size 512, we get maximum pt =33
  }
  
}

void get_pile_up_jets( Parton jets[n_pu_jets], ap_uint<n_pu_jets*(lut_size+20)> rand ){

  ap_uint<n_pu_jets*lut_size> rand_pt =rand(n_pu_jets*lut_size-1,0);
  ap_uint<n_pu_jets*10>       rand_eta=rand(n_pu_jets*(10+lut_size)-1,n_pu_jets*lut_size);
  ap_uint<n_pu_jets*10>       rand_phi=rand(n_pu_jets*(20+lut_size)-1,n_pu_jets*(10+lut_size));
  pt_t pt_exp_lut[512]; _lut_pt_exp_init( pt_exp_lut );

  
  for (int ijet=0; ijet<n_pu_jets; ++ijet){
#pragma HLS unroll
    Parton p; p.hwPartType=1;
    p.hwPt=pt_exp_lut[ int(rand_pt(lut_size*(ijet+1)-1,lut_size*ijet))];
    p.hwEta=rand_eta(nbits_etaphi*(ijet+1)-1,nbits_etaphi*ijet)-(1 << (nbits_etaphi-2));
    p.hwPhi=rand_phi(nbits_etaphi*(ijet+1)-1,nbits_etaphi*ijet)-(1 << (nbits_etaphi-2));
    jets[ijet]=p;
  }

}



void pileup_generator(Parton out_particles[n_pu_jets*pu_jet_npart], ap_uint<random_bits_per_splitting*(pu_jet_depth2*2-1)*n_pu_jets> bigrand, 
                      ap_uint<n_pu_jets*(lut_size+20)> jetrand)
{
#pragma HLS pipeline II=8
#pragma HLS array_partition variable=out_particles complete 
  //printf("Im here\n");
  Parton jets[n_pu_jets]; get_pile_up_jets( jets, jetrand);
  const pt_t min_particle_energy=2; // 0.5 in 0.25 GeV units
  shower_n_jets::helper<pu_jet_depth2,n_pu_jets>::call( jets, out_particles,  min_particle_energy, bigrand );
  //printf("Im done\n");
}
