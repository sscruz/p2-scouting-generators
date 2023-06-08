#include "gen4.h"
#include <cassert>

void gen4(bool valid, ap_uint<64> data, ap_uint<64> data_out[4], bool orbit_out[4], bool start_out[4], bool end_out[4], bool valid_out[4], ap_uint<4> CLOCK_RATIO_MINUS_ONE) {
  #pragma HLS array_partition variable = data_out complete
  #pragma HLS array_partition variable = orbit_out complete
  #pragma HLS array_partition variable = start_out complete
  #pragma HLS array_partition variable = end_out complete
  #pragma HLS array_partition variable = valid_out complete
  #pragma HLS stable variable = CLOCK_RATIO_MINUS_ONE
  #pragma HLS pipeline II = 1

  static ap_uint<12> bx_ = 0, evbx_ = 0;
  static ap_uint<4>  frag_ = 0;
  static ap_uint<8>  irow_ = 0, count_ = 0;
  static bool running_ = 0;
  static ap_uint<10> wptr_ = 0, rptr_ = 0;
  static ap_uint<8>  npuppiArr_[1024], npuppi_, ipuppi_, nremaining_;

  for (unsigned int i = 0; i < 4; ++i) {
    data_out[i] = 0;
    valid_out[i] = 0;
    orbit_out[i] = 0;
    start_out[i] = 0;
    end_out[i] = 0;
  }
  if (valid && (data(55,48).or_reduce() != 0)) {
    wptr_ = 0; rptr_ = 0;
    bx_ = data(39,32);
    count_ = data(55,48);
    npuppi_ = data(7,0);
    frag_ = 0; 
    irow_ = 0;
    running_ = true;
  } else if (running_) {
    if (irow_ == 0) {
      evbx_ = bx_;
      for (int i = 0; i < 4; ++i) {
        #pragma HLS unroll
        end_out[0]   = (npuppi_ <= 4);
        ap_uint<64> puppi = 0;
        puppi(11,0) = ap_uint<12>(i+1);
        puppi(23,12) = evbx_;
        puppi(31,24) = npuppi_;
        puppi(47,32) = ap_uint<16>(0xABC0);
        data_out[i] = (i < npuppi_) ? puppi : ap_uint<64>(0);
        orbit_out[i] = (rptr_ == 0);
        start_out[i] = true;
        end_out[i]   = (npuppi_ <= 4+i);
        valid_out[i] = true;
      }
      ipuppi_ = 4;
      irow_ = 1;
      nremaining_ = (npuppi_ > 4) ? ap_uint<8>(npuppi_ - 4) : ap_uint<8>(0);
    } else {
      for (int i = 0; i < 4; ++i) {
        #pragma HLS unroll
        ap_uint<64> puppi = 0;
        puppi(11,0) = ap_uint<12>(ipuppi_+i+1);
        puppi(23,12) = evbx_;
        puppi(31,24) = npuppi_;
        puppi(47,32) = ap_uint<16>(0xABC0);
        data_out[i] = (i < nremaining_) ? puppi : ap_uint<64>(0);
        orbit_out[i] = false;
        start_out[i] = false;
        end_out[i] = (i < nremaining_) && (i+4 >= nremaining_);
        valid_out[i] = (i < nremaining_);
      }
      nremaining_ = (nremaining_ > 4) ? ap_uint<8>(nremaining_ - 4) : ap_uint<8>(0);
      ipuppi_ += 4;
      if (irow_ == count_ - 1) {
        irow_ = 0;
        if (rptr_ != wptr_) {
          npuppi_ = npuppiArr_[rptr_];
          rptr_++;
        } else {
          running_ = false;
        }
      } else {
        irow_++;
      }
    }

    if (valid) {
      npuppiArr_[wptr_] = data(7,0);
      wptr_++;
    } 

    if (frag_ == CLOCK_RATIO_MINUS_ONE) {
      bx_++;
      frag_ = 0;
    } else {
      frag_++;
    }
  }
}