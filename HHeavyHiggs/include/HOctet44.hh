# ifndef HOctet44_hh
# define HOctet44_hh

# include "HQuartet31.hh"


struct HChargedEventStruct {

  int LeptonNumber = 0;
  int JetNumber = 0;
  int BottomNumber = 0;
  float ScalarHt = 0;
  float RestHt = 0;
  float RestM = 0;
  float RestPt = 0;
  float RestRap = 0;
  float RestPhi = 0;
  int RestBTag = 0;
  float RestBBdt = 0;
  float MaxBBdt = 0;
  float TotalBBdt = 0;
  float ThirdBBdt = 0;
  float LeptonPt = 0;

};

/**
 * @brief An octet composed of two quartets
 *
 */
class HOctet44 : public hanalysis::HTag
{

public:

    HOctet44(const hanalysis::HQuartet31& NewQuartet1, const hanalysis::HQuartet31& NewDoublet);

    HOctet44(const hanalysis::HQuartet31 &NewQuartet1, const hanalysis::HQuartet31 &NewQuartet2, const HChargedEventStruct &NewEventStruct);

    inline  fastjet::PseudoJet Jet() const{
      return (GetQuartet1Jet() + GetQuartet2Jet());
    }

    inline hanalysis::HQuartet31 Quartet1()const {
        return Quartet1M;
    }

    inline hanalysis::HQuartet31 Quartet2() const {
        return Quartet2M;
    }

    inline fastjet::PseudoJet GetQuartet1Jet() const {
        return Quartet1M.Jet();
    }

    inline fastjet::PseudoJet GetQuartet2Jet() const {
        return Quartet2M.Jet();
    }



    inline float DeltaR() const {
      return GetQuartet1Jet().delta_R(GetQuartet2Jet());
    }
    inline float DeltaPhi() const {
      return GetQuartet1Jet().delta_phi_to(GetQuartet2Jet());
    }

    inline float DeltaRap() const {
      return GetQuartet1Jet().rap() - GetQuartet2Jet().rap();
    }

    inline float DeltaPt() const {
      return GetQuartet1Jet().pt() - GetQuartet2Jet().pt();
    }

//     inline float GetDeltaPt1() const {
//         return (DeltaR() + GetDeltaR2());
//     }
//
//     inline float GetHbDeltaDeltaR() const {
//         return std::abs(DeltaR() - GetDeltaR2());
//     }
//
//     inline float GetHbSumDeltaRap() const {
//         return DeltaRap() + GetDeltaRap2();
//     }
//
//     inline float GetHbDeltaDeltaRap() const {
//         return std::abs(DeltaRap() - GetDeltaRap2());
//     }
//
//     inline float GetHbSumDeltaPhi() const {
//         return GetDeltaPhi1() + GetDeltaPhi2();
//     }
//
//     inline float GetHbDeltaDeltaPhi() const {
//         return GetDeltaPhi(GetDeltaPhi1(), GetDeltaPhi2());
//     }

inline void SetScalarHt(const float NewScalarHt) {
  EventStructM.ScalarHt = NewScalarHt;
}

inline void SetJetNumber(const int NewJetNumber) {
  EventStructM.JetNumber = NewJetNumber;
}

inline void SetBottomNumber(const int NewBottomNumber) {
  EventStructM.BottomNumber = NewBottomNumber;
}

inline void SetLeptonNumber(const int NewLeptonNumber) {
  EventStructM.LeptonNumber = NewLeptonNumber;
}

    inline float ScalarHt() const {
      return EventStructM.ScalarHt;
    }

    inline int JetNumber()const {
      return EventStructM.JetNumber;
    }

    inline int BottomNumber()const {
      return EventStructM.BottomNumber;
    }

    inline int LeptonNumber()const {
      return EventStructM.LeptonNumber;
    }

    inline HChargedEventStruct EventStruct()const {
      return EventStructM;
    }

    inline void SetEventStruct(const HChargedEventStruct &NewEventStruct) {
      EventStructM = NewEventStruct;
    }


    HChargedEventStruct EventStructM;

//     int LeptonNumber;
//
//     int BottomNumber;
//
//     int JetNumber;
//
//     int ScalarHt;

protected:

    virtual inline std::string ClassName() const {
        return "HOctet44";
    }

private:


    hanalysis::HQuartet31 Quartet1M;

    hanalysis::HQuartet31 Quartet2M;

};

#endif
