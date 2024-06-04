#ifndef eventmaker_h
#define eventmaker_h
#include"data.h"
#include <iostream>
// // "hadronizes" partons and adds vz

z0_t get_vertex_position( ap_uint<10> vertex_rand  );

// adding vertex position and its resolution. We assume the resolution to be 0.4 cm 
// using smaller luts because the range is smaller
template<unsigned int multiplicity> 
void add_vz_position( l1ct::PuppiObj out_particles[multiplicity], z0_t vz, ap_uint<8*multiplicity> random )
{
    ap_uint<9> table_gaussian[128]={0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 9, 10, 10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 14, 14, 14, 15, 15, 15, 16, 16, 16, 17, 17, 17, 18, 18, 19, 19, 19, 20, 20, 20, 21, 21, 22, 22, 22, 23, 23, 24, 24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 28, 29, 29, 30, 30, 31, 31, 32, 32, 33, 34, 34, 35, 35, 36, 36, 37, 38, 38, 39, 40, 40, 41, 42, 43, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 55, 56, 57, 59, 61, 63, 66, 68, 72, 77, 85};
    for (unsigned int i=0; i<multiplicity; ++i ){
        #pragma HLS unroll
        ap_uint<9> per_part_rand=random((i+1)*8-1,i*8);
        auto per_part_vz = per_part_rand(0,0) ? z0_t( table_gaussian[per_part_rand(7,1)]) : z0_t(-table_gaussian[per_part_rand(7,1)]);
        if ( out_particles[i].hwId.charged() && out_particles[i].hwPt != 0 ) { out_particles[i].setHwZ0( vz + per_part_vz ) ; }
    }
}

template<unsigned int multiplicity> 
void particles_to_partons(l1ct::PuppiObj out_particles[multiplicity], Parton in_partons[multiplicity],
                          ap_uint<multiplicity*10> rand){

    for (unsigned int i=0; i<multiplicity; ++i){
#pragma HLS unroll
        out_particles[i]=particle_to_parton(in_partons[i], rand((i+1)*10-1,i*10));
    }


}

template<unsigned int multiplicity>
void hadronize_event_template(Parton in_partons[multiplicity], l1ct::PuppiObj out_particles[multiplicity], 
                ap_uint<10> vertex_rand, ap_uint<8*multiplicity> vz_resolution_rand, ap_uint<10*multiplicity> hadronization_rand )
{
    z0_t vz=get_vertex_position(  vertex_rand );
    
#ifndef __SYNTHESIS__
    //    std::cout << "vertex: " <<   vz << std::endl;
#endif
    particles_to_partons<multiplicity>( out_particles, in_partons, hadronization_rand);
    add_vz_position<multiplicity>( out_particles, vz, vz_resolution_rand);

}

void hadronize_event(Parton in_partons[n_pu_jets*pu_jet_npart], l1ct::PuppiObj out_particles[n_pu_jets*pu_jet_npart], ap_uint<10> vertex_rand, ap_uint<8*n_pu_jets*pu_jet_npart> vz_resolution_rand, ap_uint<n_pu_jets*pu_jet_npart*10> hadronization_rand);

void event_maker_pileup( l1ct::PuppiObj out_particles[n_pu_jets*pu_jet_npart],  ap_uint<10> vertex_rand, ap_uint<8*n_pu_jets*pu_jet_npart> vz_resolution_rand, ap_uint<n_pu_jets*pu_jet_npart*10> hadronization_rand, ap_uint<random_bits_per_splitting*(pu_jet_depth2*2-1)*n_pu_jets> bigrand, ap_uint<n_pu_jets*(lut_size+20)>  jetrand);
void bitpattern_pileup( PackedPuppiObj out_particles[n_pu_jets*pu_jet_npart],  ap_uint<10> vertex_rand, ap_uint<8*n_pu_jets*pu_jet_npart> vz_resolution_rand, ap_uint<n_pu_jets*pu_jet_npart*10> hadronization_rand, ap_uint<random_bits_per_splitting*(pu_jet_depth2*2-1)*n_pu_jets> bigrand, ap_uint<n_pu_jets*(lut_size+20)>  jetrand);


#endif
