# pragma once

# include "Sextet.hh"

namespace analysis {

/**
 * @brief An octet composed of a sextet an a doublet
 *
 */
class Octet62 : public Identification
{

public:

    Octet62() {};

    Octet62(const Sextet &sextet, const Doublet &doublet);

    bool overlap() const;

    bool overlap(const fastjet::PseudoJet &jet) const;

    inline Sextet sextet()const {
        return sextet_;
    }

    inline Doublet doublet() const {
        return doublet_;
    }

    inline fastjet::PseudoJet sextetJet() const {
        return sextet_.Jet();
    }

    inline fastjet::PseudoJet doublet_jet() const {
        return doublet_.Jet();
    }

    inline fastjet::PseudoJet Jet() const {
        return sextetJet() + doublet_jet();
    }

    inline float Ht() const {
        return sextet_.Ht() + doublet_.Ht();
    }

    inline float DeltaPt() const {
        return sextetJet().pt() - doublet_jet().pt();
    }

    inline float DeltaHt() const {
        return sextet().Ht() - doublet().Ht();
    }

    inline float DeltaM() const {
        return sextetJet().m() - doublet_jet().m();
    }

    inline float DeltaR() const {
        return sextetJet().delta_R(doublet_jet());
    }

    inline float DeltaRap() const {
        return sextetJet().rap() - doublet_jet().rap();
    }

    inline float DeltaPhi() const {
        return sextetJet().delta_phi_to(doublet_jet());
    }

    inline float HbDeltaDeltaR() const {
        return std::abs(GetDeltaR1() - GetDeltaR2());
    }

    inline float HbDeltaDeltaRap() const {
        return std::abs(GetDeltaRap1() - GetDeltaRap2());
    }

    inline float HbDeltaDeltaPhi() const {
        return Object::DeltaPhi(GetDeltaPhi1(), GetDeltaPhi2());
    }

    inline float BottomBdt() const {
        return doublet().Singlet1().user_info<JetInfo>().Bdt() + doublet().Singlet2().user_info<JetInfo>().Bdt() + sextet().triplet1().singlet().user_info<JetInfo>().Bdt() + sextet().triplet2().singlet().user_info<JetInfo>().Bdt();
    }

    inline float PairBottomBdt() const {
        return doublet().Singlet1().user_info<JetInfo>().Bdt() + doublet().Singlet2().user_info<JetInfo>().Bdt();
    }

    inline float GetDeltaR1() const {
        return sextetJet().delta_R(doublet_.Singlet1());
    }

    inline float GetDeltaR2() const {
        return sextetJet().delta_R(doublet_.Singlet2());
    }

    inline float GetDeltaPhi1() const {
        return sextetJet().delta_phi_to(doublet_.Singlet1());
    }

    inline float GetDeltaPhi2() const {
        return sextetJet().delta_phi_to(doublet_.Singlet2());
    }

    inline float GetDeltaRap1() const {
        return (sextetJet().rap() - doublet_.Singlet1().rap());
    }

    inline float GetDeltaRap2() const {
        return (sextetJet().rap() - doublet_.Singlet2().rap());
    }

    inline float GetDeltaPt1() const {
        return (sextetJet().pt() - doublet_.Singlet1().pt());
    }

    inline float GetDeltaPt2() const {
        return (sextetJet().pt() - doublet_.Singlet2().pt());
    }

    Sextet SubMultiplet1() const {
      return sextet_;
    }

    Doublet SubMultiplet2() const {
      return doublet_;
    }

protected:

    virtual inline std::string ClassName() const {
        return "Octet62";
    }

private:

    Sextet sextet_;

    Doublet doublet_;

};

}
