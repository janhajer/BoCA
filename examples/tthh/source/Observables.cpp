/**
 * Copyright (C) 2015-2016 Jan Hajer
 */
#include <boost/range/numeric.hpp>
#include <boost/range/algorithm/count_if.hpp>

#include "boca/Event.hh"
#include "boca/multiplets/Sort.hh"
#include "boca/generic/Vector.hh"
#include "boca/generic/DEBUG_MACROS.hh"

#include "include/Observables.hh"

Observables::Observables() {}

Observables::Observables(boca::Event const& event)
{
    SetEvent(event);
}

Observables::Observables(boca::Event const& event, const std::vector<boca::Jet>& jets)
{
    SetEvent(event, jets);
}

void Observables::SetEvent(boca::Event const& event, const std::vector<boca::Jet>& jets)
{
    SetJets(jets);
    SetLeptons(event.Leptons());
    scalar_ht_ = event.ScalarHt();
    missing_et_ = event.MissingEt().Pt();
}

void Observables::SetEvent(boca::Event const& event)
{
    SetJets(event.Jets());
    SetLeptons(event.Leptons());
    scalar_ht_ = event.ScalarHt();
    missing_et_ = event.MissingEt().Pt();
}

int Observables::LeptonNumber() const
{
    INFO0;
    return leptons_.size();
}

int Observables::JetNumber() const
{
    INFO0;
    return Jets().size();
}

int Observables::BottomNumber() const
{
    INFO0;
    return boost::range::count_if(Jets(), [](boca::Jet const & jet) {
        return jet.Info().BTag();
    });
    return boost::range::count_if(Jets(), [](boca::Jet const & jet) {
        return jet.Info().Bdt() > 0.05;
    });
}

boca::Momentum Observables::ScalarHt() const
{
    INFO0;
    return scalar_ht_;
}

boca::Momentum Observables::LeptonHt() const
{
    INFO0;
    return boost::accumulate(leptons_, 0. * boca::eV, [](boca::Momentum const& ht, boca::Jet const & lepton) {
        return ht + lepton.Pt();
    });
}

boca::Momentum Observables::JetHt() const
{
    INFO0;
    return boost::accumulate(jets_, 0. * boca::eV, [](boca::Momentum const& ht, boca::Jet const & jet) {
        return ht + jet.Pt();
    });
}

boca::Energy Observables::MissingEt() const
{
    INFO0;
    return missing_et_;
}

std::vector<boca::Jet> Observables::Jets() const
{
    INFO0;
    return jets_;
}

void Observables::SetJets(std::vector<boca::Jet> const& jets)
{
    INFO0;
    jets_ = SortedByPt(jets);
}

void Observables::SetLeptons(std::vector<boca::Lepton> const& leptons)
{
    INFO0;
    leptons_ = SortedByPt(leptons);
}

boca::Momentum Observables::JetPt(std::size_t number) const
{
    return jets_.size() > number ? jets_.at(number).Pt() : boca::at_rest;
}

boca::Momentum Observables::LeptonPt(std::size_t number) const
{
    return leptons_.size() > number ? leptons_.at(number).Pt() : boca::at_rest;
}

boca::Momentum Observables::Ht() const
{
    return ScalarHt();
}

int Observables::Charge() const
{
    return boost::accumulate(Jets(), 0, [](int sum, boca::Jet const & jet) {
        return sum + jet.Info().Charge();
    });
}

boca::Jet Observables::Jet() const
{
    return jet_.Get([this]() {
        return Join(Jets());
    });
}

boca::Singlet Observables::ConstituentJet() const
{
    return constituent_jet_.Get([this]() {
      return Join(boost::accumulate(Jets(), std::vector<boca::Jet>{}, [](std::vector<boca::Jet>& sum, boca::Jet const & jet) {
            return Combine(sum, jet.Constituents());
        }));
    });
}

boca::Mass Observables::Mass() const
{
    return Jet().Mass();
}
boca::Angle Observables::DeltaRTo(const boca::PseudoJet& jet) const
{
    return Jet().DeltaRTo(jet);
}

std::vector< boca::Jet > Observables::Constituents() const
{
    return ConstituentJet().Constituents();
}
std::vector< boca::LorentzVector< boca::Momentum > > Observables::LorentzVectors() const
{
    return {};
}
