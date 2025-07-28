#include"simplia.h"
#include <cmath>
#include <hls_math.h>
#include "luts.h"

//#define DEBUG



void energy_splitting( ap_uint<lut_size> random, x_t &x_qqg, x_t &x_ggg, x_t& x_gqq ){ // precision of the random given by LUT size

  x_qqg=table_qqg[int(random)];
  x_ggg=table_ggg[int(random)];
  x_gqq=table_gqq[int(random)];
  
}
void _lut_q_over_e_init(ap_uint<10> _q_over_e_lut[512])
{

  for (int i=0; i<512; ++i){
    float e=i*128/512;
    float Q=0.1;
    _q_over_e_lut[i]=(Q/e)*1023/0.4; // maximum can be 0.4, setting that to 1023
  }
  

}
void _lut_sin_cos_init( etaphi_t table_cos[512], etaphi_t table_sin[512])
{
  for (int i=0; i<512; ++i){
    table_cos[i]=cos( i*2*3.1415/512)*512;  // cos and sin between -512 and 512=2^9
    table_sin[i]=sin( i*2*3.1415/512)*512;
  }
}


void split( Parton inp, Parton& p1, Parton& p2, ap_uint<random_bits_per_splitting> rand, const pt_t min_particle_energy)
{
#ifdef DEBUG
  printf("We are splitting particle with pt %d\n", inp.hwPt);
#endif
  if (inp.hwIsStable != 0){
    p1=inp;
    p2.hwIsStable=1; // other members are zero by default
#ifdef DEBUG
    printf("Parton is stable\n");
#endif
  
    return;
  }

  x_t x_qqg,x_ggg, x_gqq, x_select;
  bool p1_type, p2_type;
  energy_splitting( (rand & 511), x_qqg,x_ggg, x_gqq);
  switch(inp.hwPartType + inp.hwPartType*((rand & 512) != 0)){
  case 0:
    x_select=x_qqg;
    p1_type=0;
    p2_type=1;
    break;

  case 1:
    x_select=x_ggg;
    p1_type=1;
    p2_type=1;
    break;
  case 2:
    x_select=x_gqq;
    p1_type=0;
    p2_type=0;
    break;
  }

#ifdef DEBUG
  printf("Out particle types are %d %d \n", p1_type, p2_type);
  printf("Splitting value is %d ", x_select); 
#endif

  
  pt_t p1pt = (x_select     * inp.hwPt) >> 14;
  pt_t p2pt = (((1 << 14)-x_select) * inp.hwPt) >> 14;


#ifdef DEBUG
  printf("Out particle pts are %d %d \n", p1pt, p2pt);
#endif

  
  //   pt_t min_particle_energy=2*4; // 2 gev in 0.25 units // now its an input
  if (p1pt<min_particle_energy || p2pt<min_particle_energy){ // we dont branch now, but may branch in the future
    p1=inp;
    p2.hwIsStable=1; // other members are zero by default
    return;
  }

  // now we know theres going to be a branching, we compute (pt,eta,phi) of the new particles
  p1.hwPt=p1pt;
  p2.hwPt=p2pt;

  ap_uint<lut_size> phi = (rand << 10) & 511; // 10 bits have already been used, take lut_size next

  etaphi_t _table_sin[512], _table_cos[512]; _lut_sin_cos_init( _table_cos, _table_sin);
  etaphi_t cos_phi=_table_cos[phi];
  etaphi_t sin_phi=_table_cos[phi];

  ap_uint<10> _q_over_e_lut[512]; _lut_q_over_e_init( _q_over_e_lut );

  ap_uint<9> p1pt_saturated = (p1pt > 511) ? ap_uint<14>(511) : p1pt; // emissions cannot be very collinear anyhow
  ap_uint<9> p2pt_saturated = (p2pt > 511) ? ap_uint<14>(511) : p2pt;
  
  etaphi_t q_over_e_1=_q_over_e_lut[p1pt_saturated]; 
  etaphi_t q_over_e_2=_q_over_e_lut[p2pt_saturated]; 

  p1.hwEta = inp.hwEta + cos_phi*q_over_e_1*40 / 512 / 1023 ; // diving by lut units and multiplying by units (0.01)
  p2.hwEta = inp.hwEta + cos_phi*q_over_e_2*40 / 512 / 1023 ; // diving by lut units and multiplying by units (0.01)
  p1.hwPhi = inp.hwPhi + sin_phi*q_over_e_1*40 / 512 / 1023 ; // diving by lut units and multiplying by units (0.01)
  p2.hwPhi = inp.hwPhi + sin_phi*q_over_e_2*40 / 512 / 1023 ; // diving by lut units and multiplying by units (0.01)
  p1.hwPartType=p1_type;
  p2.hwPartType=p2_type;
  
  return; 
}

namespace shower_tools{

  template<unsigned int depth2, unsigned int n_random_bits, unsigned int max_depth2>
  struct helper {
    static void shower_step( const Parton in_particles[depth2],  const ap_uint<n_random_bits> rand, Parton out_particles[max_depth2*2], const pt_t min_particle_energy)
    {
#pragma HLS array_partition variable=out_particles complete
#pragma HLS inline off
#ifdef DEBUG
      printf("\n\n\n\nWe are in depth %d\n\n", depth);
#endif
      Parton next_particles[depth2*2];
      for (unsigned int ipart=0; ipart < depth2; ++ipart){
#pragma HLS unroll
	Parton p1,p2;
#ifdef DEBUG
	printf("Passing bits between %d and %d\n", random_bits_per_splitting*(ipart+1)-1, random_bits_per_splitting*ipart);
#endif 
	split( in_particles[ipart], p1, p2, rand(random_bits_per_splitting*(ipart+1)-1, random_bits_per_splitting*ipart), min_particle_energy);  // pass n least significant bits
	next_particles[ipart]=p1;
	next_particles[ipart+depth2]=p2;
      }

#ifdef DEBUG
      assert( (n_random_bits-random_bits_per_splitting*depth2) == random_bits_per_splitting*(max_depth2*2-depth2*2));
      ap_uint<random_bits_per_splitting*(max_depth2*2-depth2*2)> topass=rand(n_random_bits-1,random_bits_per_splitting*depth2);
      print_ap( rand);
      print_ap( topass);
      printf("We have %d random bits. passing bits between %d and %d to the next iteration. That should be a total of %d ", n_random_bits, n_random_bits-1, random_bits_per_splitting*depth2, random_bits_per_splitting*(max_depth2*2-depth2*2)); 
#endif 
      

      helper<depth2*2, random_bits_per_splitting*(max_depth2*2-depth2*2),max_depth2>::shower_step( next_particles, rand(n_random_bits-1,random_bits_per_splitting*depth2), out_particles, min_particle_energy);
    
    }
  };
  
  template<unsigned int max_depth2, unsigned int n_random_bits>
  struct helper<max_depth2,n_random_bits,max_depth2>{
    static void shower_step( const Parton in_particles[max_depth2/2], const ap_uint<n_random_bits> rand, Parton out_particles[max_depth2*2], pt_t min_particle_energy)
    {
#pragma HLS array_partition variable=out_particles complete
#pragma HLS inline off
#ifdef DEBUG
      printf("\n\n\n\n Final depth. We have %d random bits  \n\n", n_random_bits);
#endif
      for (unsigned int ipart=0; ipart < max_depth2; ++ipart){
#pragma HLS unroll
	Parton p1,p2;
	split( in_particles[ipart], p1, p2, rand(random_bits_per_splitting*(ipart+1)-1, random_bits_per_splitting*ipart), min_particle_energy  ); // pass n least significant bits
#ifdef DEBUG
	printf("    -> setting ipart %d and %d to %d and %d\n", ipart, ipart+max_depth2/2, p1.hwPt, p2.hwPt);
#endif
	out_particles[ipart]=p1;
	out_particles[(int) ipart+max_depth2]=p2;
	
      }

      
      return;
  
    }
  };

  
  
}

template<unsigned int max_depth2>
void shower_template(const Parton theparticle, Parton out_particles[max_depth2*2], const pt_t min_particle_energy, ap_uint<random_bits_per_splitting*(max_depth2*2-1)> rand)
{

#pragma HLS pipeline II=9
#pragma HLS array_partition variable=out_particles complete
#ifdef DEBUG
  print_ap(rand);
#endif
  Parton p[1]; p[0]=theparticle;
  shower_tools::helper<1,random_bits_per_splitting*(max_depth2*2-1),max_depth2>::shower_step( p,   rand, out_particles, min_particle_energy);

}

// random_bits_per_splitting*(max_depth2-1)

constexpr int int_ceil(const float f)
{
    return (f > static_cast<int>(f)) ? static_cast<int>(f) + 1 : static_cast<int>(f);
}



void shower(const Parton theparticle, Parton out_particles[32], ap_uint<589> rand)
{
#pragma HLS pipeline II=9
#pragma HLS array_partition variable=out_particles complete
  const pt_t min_particle_energy=0; // was 2*4
  return shower_template<16>(theparticle, out_particles, min_particle_energy, rand); // max_depth is max_depth-1
}




