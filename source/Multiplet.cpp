#include "Multiplet.hh"

#include "InfoRecombiner.hh"
#include "Vector.hh"
#include "Math.hh"
#include "Debug.hh"

namespace analysis
{

Vector2 Multiplet::Pull() const
{
    Error("do not end up here");
    return Vector2();
};

fastjet::PseudoJet Multiplet::Jet(const fastjet::PseudoJet& jet_1, const fastjet::PseudoJet& jet_2) const
{
    analysis::Jets constituents;
    if (jet_1.has_constituents()) constituents = jet_1.constituents();
    else constituents.push_back(jet_1);
    if (jet_2.has_constituents()) constituents = Join(constituents, jet_2.constituents());
    else constituents.push_back(jet_2);
    fastjet::PseudoJet jet = fastjet::join(constituents, InfoRecombiner());
    return jet;
}

float Multiplet::DeltaPt(const MultipletBase& multiplets_1, const MultipletBase& multiplets_2) const
{
    float delta_pt  = multiplets_1.Jet().pt() - multiplets_2.Jet().pt();
    if(delta_pt != delta_pt) return 0;
    return delta_pt;
}

float Multiplet::Ht(const MultipletBase& multiplets_1, const MultipletBase& multiplets_2) const
{
    return multiplets_1.Ht() + multiplets_2.Ht();
}

float Multiplet::DeltaRap(const MultipletBase& multiplets_1, const MultipletBase& multiplets_2) const
{
    float delta_rap = multiplets_1.Jet().rap() - multiplets_2.Jet().rap();
    if (std::abs(delta_rap) > 100) return 0;
    return delta_rap;
}

float Multiplet::DeltaPhi(const MultipletBase& multiplets_1, const MultipletBase& multiplets_2) const
{
    return multiplets_1.Jet().delta_phi_to(multiplets_2.Jet());
}

float Multiplet::DeltaR(const MultipletBase& multiplets_1, const MultipletBase& multiplets_2) const
{
    float delta_r = multiplets_1.Jet().delta_R(multiplets_2.Jet());
    if (std::abs(delta_r) > 100) delta_r = 0;
//         if (delta_r < DetectorGeometry::MinCellResolution()) delta_r = Singlet(Jet()).DeltaR();
    return delta_r;
}

float Multiplet::DeltaM(const MultipletBase& multiplets_1, const MultipletBase& multiplets_2) const
{
    return multiplets_1.Jet().m() - multiplets_2.Jet().m();
}

float Multiplet::DeltaHt(const MultipletBase& multiplets_1, const MultipletBase& multiplets_2) const
{
    return multiplets_1.Ht() - multiplets_2.Ht();
}

float Multiplet::Rho(const MultipletBase& jet_1, const MultipletBase& jet_2) const
{
    if (Jet(jet_1.Jet(), jet_2.Jet()).pt() < DetectorGeometry::MinCellPt() || DeltaR(jet_1, jet_2) < DetectorGeometry::MinCellResolution()) return 0;
    return Jet().m() / Jet().pt() / DeltaR(jet_1, jet_2) * 2;
}

float Multiplet::Pull(const MultipletBase& multiplets_1, const MultipletBase& multiplets_2) const
{
    Vector2 pull = multiplets_1.singlet().Pull() - multiplets_1.Reference(multiplets_2.Jet());
    return std::atan2(pull.Y(), pull.X());
}

float Multiplet::PullDifference(const MultipletBase& multiplets_1, const MultipletBase& multiplets_2) const
{
    return RestrictPhi(analysis::DeltaPhi(Pull(multiplets_1, multiplets_2), Pull(multiplets_2, multiplets_1)) - M_PI) / M_PI;
}

float Multiplet::PullSum(const MultipletBase& multiplets_1, const MultipletBase& multiplets_2) const
{
    return RestrictPhi(Pull(multiplets_1, multiplets_2) + Pull(multiplets_2, multiplets_1)) / M_PI;
}

float Multiplet::Dipolarity(const MultipletBase& multiplets_1, const MultipletBase& multiplets_2) const
{
    if (DeltaR(multiplets_1, multiplets_2) == 0) return 0;
    fastjet::PseudoJet jet = Jet(multiplets_1.Jet(), multiplets_2.Jet());
    if (jet.pt() == 0) return 0;
    float dipolarity = 0;
    if (!jet.has_constituents()) return 0;
    for (const auto & constituent : jet.constituents()) {
        if (constituent.pt() > jet.pt()) continue;

        float phi = constituent.phi_std();
        float distance_1 = Distance(multiplets_1, multiplets_2, Vector2(constituent.rap(), phi));

        if (phi < 0) phi += 2 * M_PI;
        else  phi -= 2 * M_PI;
        float distance_2 =  Distance(multiplets_1, multiplets_2, Vector2(constituent.rap(), phi));

        float distance = std::min(distance_1, distance_2);
        if (distance > DeltaR(multiplets_1, multiplets_2)) continue;
        dipolarity += constituent.pt() * sqr(distance);
    }
    return dipolarity / jet.pt() / sqr(DeltaR(multiplets_1, multiplets_2));
}

float Multiplet::Distance(const MultipletBase& multiplets_1, const MultipletBase& multiplets_2, const Vector2& point_0) const
{
    Vector2 point_1(multiplets_1.Jet().rap(), multiplets_1.Jet().phi_std());
    Vector2 point_2(multiplets_2.Jet().rap(), multiplets_2.Jet().phi_std());
    return std::abs(point_2.Y() - point_1.Y() * point_0.X() - (point_2.X() - point_1.X()) * point_0.Y() + point_2.X() * point_1.Y() - point_2.Y() * point_1.X()) / DeltaR(multiplets_1, multiplets_2);
}
int Multiplet::Charge(const MultipletBase& multiplets_1, const MultipletBase& multiplets_2) const
{
    return sgn(multiplets_1.Charge() + multiplets_2.Charge());
}

float Multiplet::BottomBdt(const MultipletBase& multiplets_1, const MultipletBase& multiplets_2) const
{
   return (multiplets_1.BottomBdt() + multiplets_2.BottomBdt()) / 2 ;
}


}