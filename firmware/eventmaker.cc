#include "eventmaker.h"
#include "pileup.h"
#include "bitonic_hybrid_sort_ref.h"

l1ct::ParticleID get_particle_pid( ap_uint<10> pid_rand){

  if (pid_rand < 3)                      return l1ct::ParticleID::MUPLUS   ;
  else if (pid_rand < 3+3)               return l1ct::ParticleID::MUMINUS  ;
  else if (pid_rand < 3+3+7)             return l1ct::ParticleID::ELEPLUS  ;
  else if (pid_rand < 3+3+7+7)           return l1ct::ParticleID::ELEMINUS ;
  else if (pid_rand < 3+3+7+7+41)        return l1ct::ParticleID::PHOTON   ; 
  else if (pid_rand < 3+3+7+7+41+37)     return l1ct::ParticleID::HADZERO  ;
  else if (pid_rand < 3+3+7+7+41+37+463) return l1ct::ParticleID::HADPLUS  ;
  else                                   return l1ct::ParticleID::HADMINUS ;
  
  
}

z0_t get_vertex_position( ap_uint<10> vertex_rand  ) // 9 bits for the magnitude and 1 for the sign
{
 
  //std::cout << "random: "  << vertex_rand << std::endl;
    // table has the quantiles of a gaussian with the right resolution, already in the right units. STD of vertex position assumed to be 4 cm
    ap_uint<9> table_gaussian[512]={  0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 9, 10, 10, 10, 10, 11, 11, 11, 11, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 15, 16, 16, 16, 16, 17, 17, 17, 17, 18, 18, 18, 19, 19, 19, 19, 20, 20, 20, 20, 21, 21, 21, 21, 22, 22, 22, 22, 23, 23, 23, 23, 24, 24, 24, 25, 25, 25, 25, 26, 26, 26, 26, 27, 27, 27, 27, 28, 28, 28, 29, 29, 29, 29, 30, 30, 30, 30, 31, 31, 31, 31, 32, 32, 32, 33, 33, 33, 33, 34, 34, 34, 34, 35, 35, 35, 36, 36, 36, 36, 37, 37, 37, 37, 38, 38, 38, 39, 39, 39, 39, 40, 40, 40, 40, 41, 41, 41, 42, 42, 42, 42, 43, 43, 43, 44, 44, 44, 44, 45, 45, 45, 46, 46, 46, 46, 47, 47, 47, 48, 48, 48, 48, 49, 49, 49, 50, 50, 50, 50, 51, 51, 51, 52, 52, 52, 52, 53, 53, 53, 54, 54, 54, 54, 55, 55, 55, 56, 56, 56, 57, 57, 57, 57, 58, 58, 58, 59, 59, 59, 60, 60, 60, 60, 61, 61, 61, 62, 62, 62, 63, 63, 63, 64, 64, 64, 64, 65, 65, 65, 66, 66, 66, 67, 67, 67, 68, 68, 68, 69, 69, 69, 70, 70, 70, 70, 71, 71, 71, 72, 72, 72, 73, 73, 73, 74, 74, 74, 75, 75, 75, 76, 76, 76, 77, 77, 77, 78, 78, 78, 79, 79, 79, 80, 80, 81, 81, 81, 82, 82, 82, 83, 83, 83, 84, 84, 84, 85, 85, 85, 86, 86, 87, 87, 87, 88, 88, 88, 89, 89, 89, 90, 90, 91, 91, 91, 92, 92, 92, 93, 93, 94, 94, 94, 95, 95, 96, 96, 96, 97, 97, 98, 98, 98, 99, 99, 100, 100, 100, 101, 101, 102, 102, 102, 103, 103, 104, 104, 104, 105, 105, 106, 106, 107, 107, 107, 108, 108, 109, 109, 110, 110, 111, 111, 111, 112, 112, 113, 113, 114, 114, 115, 115, 116, 116, 117, 117, 118, 118, 119, 119, 120, 120, 121, 121, 122, 122, 123, 123, 124, 124, 125, 125, 126, 126, 127, 127, 128, 128, 129, 129, 130, 131, 131, 132, 132, 133, 133, 134, 135, 135, 136, 136, 137, 138, 138, 139, 139, 140, 141, 141, 142, 143, 143, 144, 145, 145, 146, 147, 147, 148, 149, 149, 150, 151, 151, 152, 153, 154, 154, 155, 156, 157, 158, 158, 159, 160, 161, 162, 163, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 184, 185, 186, 187, 189, 190, 191, 193, 194, 195, 197, 198, 200, 202, 203, 205, 207, 209, 210, 212, 214, 216, 219, 221, 223, 226, 229, 232, 235, 238, 241, 245, 249, 254, 259, 265, 271, 279, 289, 303, 325};
    ap_uint<9> vertex_z = table_gaussian[vertex_rand(9,1)];
    return (vertex_rand(0,0)) ?  z0_t(vertex_z)  : z0_t(-vertex_z);
}

l1ct::PuppiObj particle_to_parton(Parton part, ap_uint<10> rand)
{
  l1ct::PuppiObj ret;
  ret.hwPt(l1ct::pt_t::width-1,0)  = part.hwPt(pt_t::width-1,0); // we can do this because they have the same precision
  ret.hwEta = part.hwEta * 4; // has two bits more
  ret.hwPhi = part.hwPhi * 2; // has one bit more 
  ret.hwId  = get_particle_pid(rand);
  return ret;

}

void hadronize_event(Parton in_part[n_pu_jets*pu_jet_npart], l1ct::PuppiObj out_particles[n_pu_jets*pu_jet_npart], ap_uint<10> vertex_rand,
                     ap_uint<8*n_pu_jets*pu_jet_npart> vz_resolution_rand, ap_uint<n_pu_jets*pu_jet_npart*10> hadronization_rand )
{
#pragma HLS pipeline II=8
#pragma HLS array_partition variable=out_particles complete 
  l1ct::PuppiObj presort[n_pu_jets*pu_jet_npart];
  hadronize_event_template<n_pu_jets*pu_jet_npart>(in_part, presort, vertex_rand, vz_resolution_rand, hadronization_rand );
  folded_hybrid_bitonic_sort_and_crop_ref(n_pu_jets*pu_jet_npart,n_pu_jets*pu_jet_npart, presort, out_particles);

}

void event_maker_pileup( l1ct::PuppiObj out_particles[n_pu_jets*pu_jet_npart],
                         ap_uint<10> vertex_rand, 
                         ap_uint<8*n_pu_jets*pu_jet_npart> vz_resolution_rand, 
                         ap_uint<n_pu_jets*pu_jet_npart*10> hadronization_rand,
                         ap_uint<random_bits_per_splitting*(pu_jet_depth2*2-1)*n_pu_jets> shower_rand,
                         ap_uint<n_pu_jets*(lut_size+20)>  jetrand)
{
#pragma HLS pipeline II=8
#pragma HLS array_partition variable=out_particles complete

  Parton partons[n_pu_jets*pu_jet_npart];
  pileup_generator(partons, shower_rand, jetrand);
  hadronize_event_template<n_pu_jets*pu_jet_npart>(partons, out_particles, vertex_rand, vz_resolution_rand, hadronization_rand);

}



void bitpattern_pileup( PackedPuppiObj bitpattern[n_pu_jets*pu_jet_npart],
                        ap_uint<10> vertex_rand,
                        ap_uint<8*n_pu_jets*pu_jet_npart> vz_resolution_rand,
                        ap_uint<n_pu_jets*pu_jet_npart*10> hadronization_rand,
                        ap_uint<random_bits_per_splitting*(pu_jet_depth2*2-1)*n_pu_jets> shower_rand,
                        ap_uint<n_pu_jets*(lut_size+20)>  jetrand)
{
#pragma HLS pipeline II=8
#pragma HLS array_partition variable=bitpattern complete

  l1ct::PuppiObj out_particles[n_pu_jets*pu_jet_npart];
  event_maker_pileup( out_particles, vertex_rand, vz_resolution_rand,
                      hadronization_rand, shower_rand, jetrand);
            
  for (int i=0; i<n_pu_jets*pu_jet_npart; ++i){
    #pragma HLS unroll 
    bitpattern[i]=out_particles[i].pack();
  }
}
