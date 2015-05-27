# include "Doublet.hh"

void analysis::Doublet::SetSinglet1(const fastjet::PseudoJet &singlet)
{
    singlet_1_ = singlet;
}

void analysis::Doublet::SetSinglet2(const fastjet::PseudoJet &singlet)
{
    singlet_2_ = singlet;
}

std::vector<analysis::Kinematics> analysis::Doublet::constituents(const fastjet::PseudoJet &jet, const float jet_ratio, const float theta, const float shift) const
{
    Print(kInformation, "constituents", jet_ratio, theta);
    const float Cut = 2. / jet_ratio;
    const float Cut1 = 1. / jet_ratio;
    std::vector<Kinematics> Newconstituents;
    for (const auto & constituentJet : jet.constituents()) {
        if (jet.delta_R(constituentJet) > Cut) continue;
        if (jet.delta_R(constituentJet) < Cut1) continue;
        // Get constituent coordinates in Jet coordinates
        const float ConstRap = constituentJet.rap() - jet.rap();
        const float ConstPhi = Object::DeltaPhi(constituentJet.phi_std(), jet.phi_std());
        if (ConstPhi > Cut) Print(kError, "phi", "too big");
        if (ConstRap > Cut) Print(kError, "eta", "too big");
        // rotate constituent according to other jet
        float ObservableRap = ConstRap * cos(theta) + ConstPhi * sin(theta);
        float ObservablePhi = ConstRap * sin(theta) - ConstPhi * cos(theta);
        // Scale according to Jet distance
        ObservableRap *= jet_ratio;
        ObservablePhi *= jet_ratio;
        // move jet to (+-1,0)
        ObservableRap -= shift;
        Print(kDebug, "eta", ObservableRap);
        Kinematics constituent(constituentJet.pt(), ObservableRap, ObservablePhi);
        Newconstituents.emplace_back(constituent);
    }
    return Newconstituents;
}
analysis::Doublet::Doublet()
{
    Print(kInformation, "Constructor");
}
analysis::Doublet::Doublet(const fastjet::PseudoJet &singlet_1, const fastjet::PseudoJet &singlet_2)
{
    Print(kInformation, "Constructor");
    SetSinglets(singlet_1, singlet_2);
}

analysis::Doublet::Doublet(const fastjet::PseudoJet &singlet)
{
    Print(kInformation, "Constructor");
    SetSinglets(singlet);
}

fastjet::PseudoJet analysis::Doublet::Singlet1()const
{
    return singlet_1_;
}

fastjet::PseudoJet analysis::Doublet::Singlet2()const
{
    return singlet_2_;
}

void analysis::Doublet::SetSinglets(const fastjet::PseudoJet &singlet_1, const fastjet::PseudoJet &singlet_2)
{
    Print(kInformation, "Constructor");
    SetSinglet1(singlet_1);
    SetSinglet2(singlet_2);
    if (Singlet1().has_user_info<JetInfo>() && Singlet2().has_user_info<JetInfo>()) SetBdt(Singlet1().user_info<JetInfo>().Bdt(),Singlet2().user_info<JetInfo>().Bdt());
    else if (Singlet1().has_user_info<JetInfo>()) SetBdt(Singlet1().user_info<JetInfo>().Bdt());
    else if (Singlet2().has_user_info<JetInfo>()) SetBdt(Singlet2().user_info<JetInfo>().Bdt());
}

void analysis::Doublet::SetSinglets(const fastjet::PseudoJet &singlet)
{
    Print(kInformation, "Constructor");
    SetSinglet1(singlet / 2);
    SetSinglet2(singlet / 2);
    SetDegenerate();
    if (singlet.has_user_info<JetInfo>() && singlet.user_info<JetInfo>().Bdt() != initial_value()) SetBdt(singlet.user_info<JetInfo>().Bdt());
    else SetBdt(0);
    if (singlet.has_user_info<JetInfo>() && singlet.user_info<JetInfo>().Tag() != initial_value()) SetTag(singlet.user_info<JetInfo>().Tag());
}

float analysis::Doublet::ReferenceAngle(const fastjet::PseudoJet &NewJet, const fastjet::PseudoJet &ReferenceJet) const
{
    Print(kInformation, "ReferenceAngle");
    const float Rap = NewJet.rap() - ReferenceJet.rap();
    const float Phi = NewJet.delta_phi_to(ReferenceJet);
    return std::atan2(-Phi, -Rap);
}


float analysis::Doublet::PullAngle1() const
{
    Print(kInformation, "PullAngle1");
//     const float NewPull = static_cast<DoubletPrivate *>(TagPrivate.get())->Pull(Singlet1());
    const float NewPull = Pull(Singlet1());
//     const float NewReferenceAngle = static_cast<DoubletPrivate *>(TagPrivate.get())->ReferenceAngle(Singlet1(), Singlet2());
    const float NewReferenceAngle = ReferenceAngle(Singlet1(), Singlet2());
    //     Print(kDebug, "Pull", Pull, ReferenceAngle, GetDeltaPhi(Pull, ReferenceAngle));
    //     if (std::abs(GetDeltaPhi(Pull, ReferenceAngle) > 3)) {
    //         Print(kError, "extrema", Pull, ReferenceAngle);
    //         Print(kError, "extrema", Jet1.rap(), Jet1.phi_std());
    //         Print(kError, "extrema", Jet2.rap(), Jet2.phi_std());
    //         Print(kError, " ");
    //     }
    return Object::DeltaPhi(NewPull, NewReferenceAngle);
}

float analysis::Doublet::PullAngle2() const
{
    Print(kInformation, "PullAngle2");
//     const float NewPull = static_cast<DoubletPrivate *>(TagPrivate.get())->Pull(Singlet2());
    const float NewPull = Pull(Singlet2());
//     const float NewReferenceAngle = static_cast<DoubletPrivate *>(TagPrivate.get())->ReferenceAngle(Singlet2(), Singlet1());
    const float NewReferenceAngle = ReferenceAngle(Singlet2(), Singlet1());
    Print(kDebug, "Pull", NewPull, NewReferenceAngle, Object::DeltaPhi(NewPull, NewReferenceAngle));
    return Object::DeltaPhi(NewPull, NewReferenceAngle);
}


float analysis::Doublet::Pull(const fastjet::PseudoJet &NewJet) const
{
    Print(kInformation, "Pull");
    float Rap = 0;
    float Phi = 0;
    for (const auto & constituent : NewJet.constituents()) {
        const float NewDeltaRap = constituent.rap() - NewJet.rap();
        const float NewDeltaPhi = Object::DeltaPhi(constituent.phi_std(), NewJet.phi_std());
        const float NewDeltaR = std::sqrt(std::pow(NewDeltaRap, 2) + std::pow(NewDeltaPhi, 2));
        const float PullFactor = constituent.pt() / NewJet.pt() * NewDeltaR;
        //         const float PullFactor = constituent.pt() / CandidateJet.pt() * constituent.delta_R(CandidateJet);
        Rap += (PullFactor * NewDeltaRap);
        Phi += (PullFactor * NewDeltaPhi);
    }
    return std::atan2(Phi, Rap);
}


std::vector<analysis::Kinematics> analysis::Doublet::constituents() const
{
    Print(kInformation, "constituents");
    if (Singlet1().constituents().empty() || Singlet2().constituents().empty()) {
        Print(kNotification, "Not enough constituents", Singlet1().constituents().size(), Singlet2().constituents().size());
        //         return 0;
    }
    const float Shift = 1;
    const float CenterRap = (Singlet1().rap() + Singlet2().rap()) / 2;
    //     const float CenterPhi = (Jet1.phi_std() + Jet2.phi_std()) / 2;
    const float CenterPhi = Object::DeltaPhi(Singlet1().phi_std(), -Singlet2().phi_std()) / 2;
    const float Theta = atan2(Object::DeltaPhi(Singlet1().phi_std(), CenterPhi), Singlet1().rap() - CenterRap);
    const float Distance = Singlet1().delta_R(Singlet2());
    const float SubJetRatio = 2. * Shift / Distance;
//     std::vector<Kinematics> constituentVectors1 = static_cast<DoubletPrivate *>(TagPrivate.get())->constituents(Singlet1(), SubJetRatio, Theta, -Shift);
    std::vector<Kinematics> constituentVectors1 = constituents(Singlet1(), SubJetRatio, Theta, -Shift);
//     std::vector<Kinematics> constituentVectors2 = static_cast<DoubletPrivate *>(TagPrivate.get())->constituents(Singlet2(), SubJetRatio, -Theta, Shift);
    std::vector<Kinematics> constituentVectors2 = constituents(Singlet2(), SubJetRatio, -Theta, Shift);
    constituentVectors1.insert(constituentVectors1.end(), constituentVectors2.begin(), constituentVectors2.end());
    return constituentVectors1;
}
