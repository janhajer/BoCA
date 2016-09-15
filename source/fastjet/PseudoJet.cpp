/**
 * Copyright (C) 2015-2016 Jan Hajer
 */

#include "TLorentzVector.h"
#include "boca/math/LorentzVector.hh"
#include "boca/fastjet/PseudoJet.hh"
// #define DEBUGGING
#include "boca/generic/DEBUG_MACROS.hh"

namespace boca
{

PseudoJet::PseudoJet() :
    fastjet::PseudoJet()
{
    INFO0;
}

PseudoJet::PseudoJet(TLorentzVector const& vector) :
    fastjet::PseudoJet(vector.Px(), vector.Py(), vector.Pz(), vector.E())
{
    INFO(vector.Px(), px(), vector.Py(), py());
}

PseudoJet::PseudoJet(LorentzVector<Momentum> const& vector) :
    fastjet::PseudoJet(vector.Px() / GeV, vector.Py() / GeV, vector.Pz() / GeV, vector.E() / GeV)
{
    INFO0;
}

void PseudoJet::ScaleMomentum(double factor)
{
    reset_momentum(px() * factor, py() * factor, pz() * factor, e());
}

const PseudoJet::UserInfoBase& PseudoJet::Info() const
{
    ERROR("probably wrong user info");
    return *new UserInfoBase;
}

PseudoJet::UserInfoBase& PseudoJet::Info()
{
    ERROR("probably wrong user info");
    return *new UserInfoBase;
}

LorentzVector< Momentum > PseudoJet::Vector() const
{
    return {Px(), Py(), Pz(), Energy()};
}

Vector3< Momentum > PseudoJet::Vector3() const
{
    return Vector().Vector();
}

Momentum PseudoJet::Pt() const
{
    return pt() * GeV;
}

Mass PseudoJet::Mass() const
{
    return m() > 0 ? m() * GeV : massless;
}

Energy PseudoJet::Energy() const
{
    return e() * GeV;
}

boca::MassSquare PseudoJet::MassSquare() const
{
    return m2() * GeV2;
}

Momentum PseudoJet::Py() const
{
    return py() * GeV;
}

Momentum PseudoJet::Pz() const
{
    return pz() * GeV;
}

Momentum PseudoJet::Px() const
{
    return px() * GeV;
}

MomentumSquare PseudoJet::ModP2() const
{
    return modp2() * GeV2;
}

Momentum PseudoJet::ModP() const
{
    return modp() * GeV;
}

Angle PseudoJet::Phi() const
{
    return phi_std() * rad;
}

Angle PseudoJet::Rap() const
{
    return rap() == fastjet::pseudojet_invalid_rap ? 0_rad : rap() * rad;
}

Angle PseudoJet::DeltaPhiTo(PseudoJet const& jet) const
{
    return delta_phi_to(jet) * rad;
}

Angle PseudoJet::DeltaRapTo(PseudoJet const& jet) const
{
    return jet.Rap() - Rap();
}

Angle PseudoJet::DeltaRTo(PseudoJet const& jet) const
{
    auto delta_r = delta_R(jet);
    return delta_r == fastjet::pseudojet_invalid_rap || delta_r > 100 ? 0_rad : delta_r * rad;
}

Vector2< Angle > PseudoJet::DeltaTo(PseudoJet const& jet) const
{
    return {DeltaRapTo(jet), DeltaPhiTo(jet)};
}

Vector2< Angle > PseudoJet::Angles(bool wrap_phi) const
{
    return {Rap(), wrap_phi ? Wrap(Phi()) : Phi()};
}

Vector2<Angle> PseudoJet::AnglesMinTo(PseudoJet const& jet) const
{
    auto angles = jet.Angles();
    auto angles_1 = Angles(false);
    auto angles_2 = Angles(true);
    return angles - angles_1 < angles - angles_2  ?  angles_1 : angles_2;
}

}
