#ifndef simplia_h
#define simplia_h

#include "data.h"




template<unsigned int max_depth2>
void shower_template(const Parton theparticle, Parton out_particles[max_depth2*2], const pt_t min_particle_energy, ap_uint<random_bits_per_splitting*(max_depth2*2-1)> rand);

void shower(const Parton theparticle, Parton outparticles[32], const ap_uint<589>);


namespace shower_n_jets{
    template<unsigned int max_depth2, unsigned int njets>
    struct helper{
        static void call( const Parton jets[njets], Parton out_particles[max_depth2*2*njets], const pt_t min_particle_energy, ap_uint<random_bits_per_splitting*(max_depth2*2-1)> bigrand[njets])
        {
            constexpr int random_bits_per_jet=random_bits_per_splitting*(max_depth2*2-1);
            for (int ijet=0; ijet<njets; ++ijet){
                #pragma HLS unroll
                Parton jetparticles[max_depth2*2];
                ap_uint<random_bits_per_jet> smallrand= bigrand[ijet];
                shower_template<max_depth2>( jets[ijet],  jetparticles, min_particle_energy, smallrand);
                for (int ipart=0; ipart<max_depth2*2; ++ipart){
                    out_particles[ipart+ijet*max_depth2*2]=jetparticles[ipart];
                }    
            }
        }
  };
  template 
  struct helper<pu_jet_depth2,n_pu_jets>;
}


#endif 
