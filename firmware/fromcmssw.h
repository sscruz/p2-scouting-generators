#ifndef ALGO_FROMCMSSW_H
#define ALGO_FROMCMSSW_H

namespace l1ct {

  typedef ap_ufixed<14, 12, AP_TRN, AP_SAT> pt_t;
  typedef ap_int<12> glbeta_t;
  typedef ap_int<11> glbphi_t;
  typedef ap_int<10> z0_t;         // 40cm / 0.1
  typedef ap_int<8> dxy_t;         // tbd
  typedef ap_uint<3> tkquality_t;  // tbd
  typedef ap_ufixed<9, 1, AP_RND_CONV, AP_WRAP> puppiWgt_t;
  typedef ap_uint<6> emid_t;

struct ParticleID {
  ap_uint<3> bits;
  enum PID {
            NONE = 0,
            HADZERO = 0,
            PHOTON = 1,
            HADMINUS = 2,
            HADPLUS = 3,
            ELEMINUS = 4,
            ELEPLUS = 5,
            MUMINUS = 6,
            MUPLUS = 7
  };
  enum PTYPE { HAD = 0, EM = 1, MU = 2 };
  
ParticleID(PID val = NONE) : bits(val) {}
  ParticleID &operator=(PID val)
    {
     bits = val;
     return *this;
    }
    
    int rawId() const{ return bits.to_int(); }
  
  bool isPhoton() const
  {
#ifndef __SYNTHESIS__
   assert(neutral());
#endif
   return bits[0];
  }
  bool isMuon() const { return bits[2] && bits[1]; }
  bool isElectron() const { return bits[2] && !bits[1]; }
  bool isChargedHadron() const { return !bits[2] && bits[1]; }
  bool charge() const {
#ifndef __SYNTHESIS__
                       assert(charged());
#endif
                       return bits[0]; /* 1 if positive, 0 if negative */
  }
  
  bool chargeOrNull() const {  // doesn't throw on null id
                             return bits[0];
  }
  bool charged() const { return bits[1] || bits[2]; };
  bool neutral() const { return !charged(); }
  int intCharge() const { return charged() ? (charge() ? +1 : -1) : 0; }
  
  void clear() { bits = 0; }

  static ParticleID mkChHad(bool charge) { return ParticleID(charge ? HADPLUS : HADMINUS); }
  static ParticleID mkElectron(bool charge) { return ParticleID(charge ? ELEPLUS : ELEMINUS); }
  static ParticleID mkMuon(bool charge) { return ParticleID(charge ? MUPLUS : MUMINUS); }
  
  inline bool operator==(const ParticleID &other) const { return bits == other.bits; }
  
    inline int pdgId() const {
      switch (bits.to_int()) {
        case HADZERO:
          return 130;
        case PHOTON:
          return 22;
        case HADMINUS:
          return -211;
        case HADPLUS:
          return +211;
        case ELEMINUS:
          return +11;
        case ELEPLUS:
          return -11;
        case MUMINUS:
          return +13;
        case MUPLUS:
          return -13;
      }
      return 0;
    }
        inline int oldId() const {
      //{ PID_Charged=0, PID_Neutral=1, PID_Photon=2, PID_Electron=3, PID_Muon=4 };
      switch (bits.to_int()) {
        case HADZERO:
          return 1;
        case PHOTON:
          return 2;
        case HADMINUS:
          return 0;
        case HADPLUS:
          return 0;
        case ELEMINUS:
          return 3;
        case ELEPLUS:
          return 3;
        case MUMINUS:
          return 4;
        case MUPLUS:
          return 4;
      }
      return -1;
    }
  };

template <typename U, typename T>
  inline void pack_into_bits(U& u, unsigned int& start, const T& data) {
  const unsigned int w = T::width;
  u(start + w - 1, start) = data(w - 1, 0);
  start += w;
}

  template <typename U, typename T>
  inline void unpack_from_bits(const U& u, unsigned int& start, T& data) {
    const unsigned int w = T::width;
    data(w - 1, 0) = u(start + w - 1, start);
    start += w;
  }

  struct PuppiObj {
    pt_t hwPt;
    glbeta_t hwEta;  // wider range to support global coordinates
    glbphi_t hwPhi;
    ParticleID hwId;

    static const int BITS_Z0_START = 0;
    static const int BITS_DXY_START = BITS_Z0_START + z0_t::width;
    static const int BITS_TKQUAL_START = BITS_DXY_START + dxy_t::width;
    static const int DATA_CHARGED_BITS_TOTAL = BITS_TKQUAL_START + tkquality_t::width;

    static const int BITS_PUPPIW_START = 0;
    static const int BITS_EMID_START = BITS_PUPPIW_START + puppiWgt_t::width;
    static const int DATA_NEUTRAL_BITS_TOTAL = BITS_EMID_START + emid_t::width;

    static const int DATA_BITS_TOTAL =
        DATA_CHARGED_BITS_TOTAL >= DATA_NEUTRAL_BITS_TOTAL ? DATA_CHARGED_BITS_TOTAL : DATA_NEUTRAL_BITS_TOTAL;

    ap_uint<DATA_BITS_TOTAL> hwData;

    inline z0_t hwZ0() const {
#ifndef __SYNTHESIS__
      assert(hwId.charged() || hwPt == 0);
#endif
      return z0_t(hwData(BITS_Z0_START + z0_t::width - 1, BITS_Z0_START));
    }

    inline void setHwZ0(z0_t z0) {
#ifndef __SYNTHESIS__
      assert(hwId.charged() || hwPt == 0);
#endif
      hwData(BITS_Z0_START + z0_t::width - 1, BITS_Z0_START) = z0(z0_t::width - 1, 0);
    }

    inline dxy_t hwDxy() const {
#ifndef __SYNTHESIS__
      assert(hwId.charged() || hwPt == 0);
#endif
      return dxy_t(hwData(BITS_DXY_START + dxy_t::width - 1, BITS_DXY_START));
    }

    inline void setHwDxy(dxy_t dxy) {
#ifndef __SYNTHESIS__
      assert(hwId.charged() || hwPt == 0);
#endif
      hwData(BITS_DXY_START + dxy_t::width - 1, BITS_DXY_START) = dxy(7, 0);
    }

    inline tkquality_t hwTkQuality() const {
#ifndef __SYNTHESIS__
      assert(hwId.charged() || hwPt == 0);
#endif
      return tkquality_t(hwData(BITS_TKQUAL_START + tkquality_t::width - 1, BITS_TKQUAL_START));
    }

    inline void setHwTkQuality(tkquality_t qual) {
#ifndef __SYNTHESIS__
      assert(hwId.charged() || hwPt == 0);
#endif
      hwData(BITS_TKQUAL_START + tkquality_t::width - 1, BITS_TKQUAL_START) = qual(tkquality_t::width - 1, 0);
    }

    inline puppiWgt_t hwPuppiW() const {
#ifndef __SYNTHESIS__
      assert(hwId.neutral());
#endif
      puppiWgt_t ret;
      ret(puppiWgt_t::width - 1, 0) = hwData(BITS_PUPPIW_START + puppiWgt_t::width - 1, BITS_PUPPIW_START);
      return ret;
    }

    inline void setHwPuppiW(puppiWgt_t w) {
#ifndef __SYNTHESIS__
      assert(hwId.neutral());
#endif
      hwData(BITS_PUPPIW_START + puppiWgt_t::width - 1, BITS_PUPPIW_START) = w(puppiWgt_t::width - 1, 0);
    }

    inline emid_t hwEmID() const {
#ifndef __SYNTHESIS__
      assert(hwId.neutral());
#endif
      return emid_t(hwData(BITS_EMID_START + emid_t::width - 1, BITS_EMID_START));
    }

    inline void setHwEmID(emid_t w) {
#ifndef __SYNTHESIS__
      assert(hwId.neutral());
#endif
      hwData(BITS_EMID_START + emid_t::width - 1, BITS_EMID_START) = w(emid_t::width - 1, 0);
    }

    inline bool operator==(const PuppiObj &other) const {
      return hwPt == other.hwPt && hwEta == other.hwEta && hwPhi == other.hwPhi && hwId == other.hwId &&
             hwData == other.hwData;
    }

    inline bool operator>(const PuppiObj &other) const { return hwPt > other.hwPt; }
    inline bool operator<(const PuppiObj &other) const { return hwPt < other.hwPt; }

    inline void clear() {
      hwPt = 0;
      hwEta = 0;
      hwPhi = 0;
      hwId.clear();
      hwData = 0;
    }

    // inline void fill(const PFRegion &region, const PFChargedObj &src) {
    //   hwEta = region.hwGlbEta(src.hwVtxEta());
    //   hwPhi = region.hwGlbPhi(src.hwVtxPhi());
    //   hwId = src.hwId;
    //   hwPt = src.hwPt;
    //   hwData = 0;
    //   setHwZ0(src.hwZ0);
    //   setHwDxy(src.hwDxy);
    //   setHwTkQuality(src.hwTkQuality);
    // }
    // inline void fill(const PFRegion &region, const PFNeutralObj &src, pt_t puppiPt, puppiWgt_t puppiWgt) {
    //   hwEta = region.hwGlbEta(src.hwEta);
    //   hwPhi = region.hwGlbPhi(src.hwPhi);
    //   hwId = src.hwId;
    //   hwPt = puppiPt;
    //   hwData = 0;
    //   setHwPuppiW(puppiWgt);
    //   setHwEmID(src.hwEmID);
    // }
    // inline void fill(const PFRegion &region, const HadCaloObj &src, pt_t puppiPt, puppiWgt_t puppiWgt) {
    //   hwEta = region.hwGlbEta(src.hwEta);
    //   hwPhi = region.hwGlbPhi(src.hwPhi);
    //   hwId = src.hwIsEM() ? ParticleID::PHOTON : ParticleID::HADZERO;
    //   hwPt = puppiPt;
    //   hwData = 0;
    //   setHwPuppiW(puppiWgt);
    //   setHwEmID(src.hwEmID);
    // }

    int intPt() const { 
      ap_uint<pt_t::width> rawPt = hwPt.range();
      return rawPt.to_int();
    }
    int intEta() const { return hwEta.to_int(); }
    int intPhi() const { return hwPhi.to_int(); }
    // float floatPt() const { return Scales::floatPt(hwPt); }
    // float floatEta() const { return Scales::floatEta(hwEta); }
    // float floatPhi() const { return Scales::floatPhi(hwPhi); }
    int intId() const { return hwId.rawId(); }
    int pdgId() const { return hwId.pdgId(); }
    int oldId() const { return hwPt > 0 ? hwId.oldId() : 0; }
    int intCharge() const { return hwId.intCharge(); }
    // float floatZ0() const { return Scales::floatZ0(hwZ0()); }
    // float floatDxy() const { return Scales::floatDxy(hwDxy()); }
    // float floatPuppiW() const { return hwId.neutral() ? Scales::floatPuppiW(hwPuppiW()) : 1.0f; }

    static const int BITWIDTH = pt_t::width + glbeta_t::width + glbphi_t::width + 3 + DATA_BITS_TOTAL;
    inline ap_uint<BITWIDTH> pack() const {
      ap_uint<BITWIDTH> ret;
      unsigned int start = 0;
      pack_into_bits(ret, start, hwPt);
      pack_into_bits(ret, start, hwEta);
      pack_into_bits(ret, start, hwPhi);
      pack_into_bits(ret, start, hwId.bits);
      pack_into_bits(ret, start, hwData);
      return ret;
    }
    inline void initFromBits(const ap_uint<BITWIDTH> &src) {
      unsigned int start = 0;
      unpack_from_bits(src, start, hwPt);
      unpack_from_bits(src, start, hwEta);
      unpack_from_bits(src, start, hwPhi);
      unpack_from_bits(src, start, hwId.bits);
      unpack_from_bits(src, start, hwData);
    }
    inline static PuppiObj unpack(const ap_uint<BITWIDTH> &src) {
      PuppiObj ret;
      ret.initFromBits(src);
      return ret;
    }
  };
} // end namespace l1ct 

#endif
