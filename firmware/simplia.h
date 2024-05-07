#ifndef simplia_h
#define simplia_h

#include "data.h"
#include "xoshiro256.h"

#define random_bits_per_splitting 19
#define lut_size 9

template<int nbits>
void print_ap( ap_uint<nbits> number){
  for (int i=0; i<nbits; ++i){
    printf("%d",number.test(i));
  }
  printf("\n");
}


void shower(const Particle theparticle[1], Particle outparticles[32]);

template<unsigned int max_depth,unsigned int max_depth2, unsigned int generator_offset>
void shower_template(const Particle theparticle[1], Particle out_particles[max_depth2*2], const pt_t min_particle_energy);



namespace shower_n_jets{

  template<unsigned int max_depth,unsigned int max_depth2, unsigned int generator_offset, unsigned int njets>
  struct helper{
    static void call(const Particle jets[njets], Particle out_particles[max_depth2*2*njets], const pt_t min_particle_energy)
    {
      

      Particle p1[1]; p1[0]=jets[njets-1];
      Particle jetparticles[max_depth2*2];
      shower_template<max_depth,max_depth2,generator_offset>( p1,  jetparticles, min_particle_energy);
      for (int i=0; i<max_depth2*2; i++){
	out_particles[i+(njets-1)*max_depth2*2]=jetparticles[i];
      }

      helper<max_depth,max_depth2,generator_offset+1,njets-1>::call( jets, out_particles, min_particle_energy );
    }
  };
      
  template<unsigned int max_depth,unsigned int max_depth2, unsigned int generator_offset>
  struct helper<max_depth,max_depth2,generator_offset,1>{
    static void call(const Particle jets[1], Particle out_particles[max_depth2*2], const pt_t min_particle_energy){
      shower_template<max_depth,max_depth2,generator_offset>( jets,  out_particles, min_particle_energy);
    }
  };

  template
  struct helper<pu_jet_depth,pu_jet_depth2,6,n_pu_jets>;

}




#endif 
