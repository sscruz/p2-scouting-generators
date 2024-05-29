#ifndef simplia_h
#define simplia_h

#include "data.h"

#define random_bits_per_splitting 19
#define lut_size 9



template<unsigned int max_depth2>
void shower_template(const Particle theparticle, Particle out_particles[max_depth2*2], const pt_t min_particle_energy, ap_uint<random_bits_per_splitting*(max_depth2*2-1)> rand);

void shower(const Particle theparticle, Particle outparticles[32], const ap_uint<589>);


namespace shower_n_jets{
    template<unsigned int max_depth2, unsigned int njets>
    struct helper{
        static void call( const Particle jets[njets], Particle out_particles[max_depth2*2*njets], const pt_t min_particle_energy, ap_uint<random_bits_per_splitting*(max_depth2*2-1)*njets> bigrand)
        {
            constexpr int random_bits_per_jet=random_bits_per_splitting*(max_depth2*2-1);
    
            for (int ijet=0; ijet<njets; ++ijet){
                #pragma HLS unroll
                Particle jetparticles[max_depth2*2];
                ap_uint<random_bits_per_jet> smallrand= bigrand(random_bits_per_jet*(ijet+1)-1,random_bits_per_jet*ijet);
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
