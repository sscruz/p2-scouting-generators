#include "firmware/gen4.h"
#include <cstdio>
#include <deque>

class Gen4Ref {
  public:
    Gen4Ref() : running_(false) {}
    void operator()(bool valid, ap_uint<64> data, ap_uint<64> data_out[4], bool orbit_out[4], bool start_out[4], bool end_out[4], bool valid_out[4]) {
      for (unsigned int i = 0; i < 4; ++i) {
        data_out[i] = 0;
        valid_out[i] = 0;
        orbit_out[i] = 0;
        start_out[i] = 0;
        end_out[i] = 0;
      }
      if (valid && !running_) {
        offset_ = (data.to_uint64() >> 32) & 0xFF;
        count_ = (data.to_uint64() >> 48) & 0xFF;
        event_ = 0;
        frame_ = 0;
        npuppi_.clear();
        npuppi_.push_back(data.to_uint64() & 0xFF);
        running_ = true;
      } else if (running_) {
        if (valid)
          npuppi_.push_back(data.to_uint64() & 0xFF);
        unsigned int irow = frame_ % count_;
        assert(!npuppi_.empty());
        unsigned int npuppi = npuppi_.front();
        unsigned int bx = (frame_ - irow) / 9 + offset_;
        for (unsigned int i = irow*4, j = 0; j < 4; ++i, ++j) {
          valid_out[j] = (irow == 0 || i < npuppi);
          data_out[j]  = (i < npuppi ? ((i + 1) + (bx << 12) + (npuppi << 24) + (0xABC0llu << 32)) : 0llu);
          orbit_out[j] = (frame_  == 0);
          start_out[j] = (irow == 0);
          end_out[j]   = valid_out[j] && (i+4 >= npuppi);
        }
        if (irow == count_ - 1) {
          npuppi_.pop_front();
          running_ = !(npuppi_.empty());
        }
        frame_++;
      }
    }

  private:
    bool running_;
    unsigned int offset_, count_, event_, frame_;
    std::deque<unsigned int> npuppi_;
};

int main(int argc, char **argv) {
  Gen4Ref gen4ref;

  bool valid; 
  ap_uint<64> data = 0;
  ap_uint<64> data_out[4], data_ref[4];
  bool orbit_out[4], orbit_ref[4];
  bool start_out[4], start_ref[4];
  bool end_out[4], end_ref[4];
  bool valid_out[4], valid_ref[4];

  unsigned int nerr = 0;
  ap_uint<32> orbit = 0x3742;
  for (unsigned int frame = 0; frame < 2000; ++frame) {
    if (frame <= 5) {
      valid = false;
    } else if (frame == 6) {
      valid = true;
      uint64_t offset = 0, count = 54, n = 10;
      data = (count << 48) + (offset << 32) + n;
    } else if (frame == 7) {
      valid = true;
      data = 54;
    } else if (frame == 8) {
      valid = true;
      data = 3;
    } else if (frame == 10) {
      valid = true;
      data = 0;
    } else if (frame <= 12) {
      valid = true;
      data = 10+2*(frame-6);
   } else {
      valid = false;
    }
    gen4(valid, data, data_out, orbit_out, start_out, end_out, valid_out, /*CLOCK_RATIO_MINUS_ONE=*/9-1);
    gen4ref(valid, data, data_ref, orbit_ref, start_ref, end_ref, valid_ref);
    printf("%07u: %1dv%016llx  |  ", frame, int(valid), data.to_uint64());
    for (int j = 0; j < 4; ++j) {
      printf("%1d%1d%1d%1d.%016llx  ", int(orbit_ref[j]), int(start_ref[j]), int(end_ref[j]), int(valid_ref[j]), data_ref[j].to_uint64());
    }
    printf("|  ");
    for (int j = 0; j < 4; ++j) {
      printf("%1d%1d%1d%1d.%016llx  ", int(orbit_out[j]), int(start_out[j]), int(end_out[j]), int(valid_out[j]), data_out[j].to_uint64());
    }
    bool ok = true;
    for (int j = 0; j < 4; ++j) {
      if (orbit_out[j] != orbit_ref[j]) ok = false;
      if (start_out[j] != start_ref[j]) ok = false;
      if (end_out[j] != end_ref[j]) ok = false;
      if (valid_ref[j] && (data_out[j] != data_ref[j])) ok = false;
    }
    if (ok) {
      printf("\n");
    } else {
      printf("<<< ERROR \n");
      if (nerr++ > 4) return 1;
    }
  }

  return (nerr > 0 ? 1 : 0);
}