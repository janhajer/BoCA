/**
 * Copyright (C) 2015-2016 Jan Hajer
 */
#include "boca/multiplets/Sort.hh"
#include "boca/multiplets/Particles.hh"
#include "boca/standardmodel/tagger/Tau.hh"
#include "boca/Event.hh"
#include "boca/generic/DEBUG_MACROS.hh"

namespace boca
{

namespace standardmodel
{

namespace tagger
{

int Tau::Train(Event const& event, PreCuts const&, Tag tag)
{
    INFO0;
    auto jets = event.Jets();
    INFO(jets.size());
    auto Particles = event.GenParticles();
    Particles = CopyIfParticle(Particles, Id::tau);
//     Particles.erase(std::remove_if(Particles.begin(), Particles.end(), WrongAbsId(Id::tau)), Particles.end());
//     if(Particles.size()!=1)
    INFO(Particles.size());
    auto final_jets = CleanJets(jets, Particles, tag);
//     if(FinalJets.size()!=1)
    INFO(final_jets.size());
//    std::vector<Jet> Pieces = GetSubJets(jets, Particles, Tag, 2);
//     FinalJets.insert(FinalJets.end(), Pieces.begin(), Pieces.end());
//
//    std::vector<Jet> Pieces2 = GetSubJets(jets, Particles, Tag, 3);
//     FinalJets.insert(FinalJets.end(), Pieces2.begin(), Pieces2.end());
    std::vector<Singlet> singlets;
    for (auto const & final_jet : final_jets) singlets.emplace_back(Singlet(final_jet));
    return SaveEntries(singlets);
}


std::vector<Jet> Tau::CleanJets(std::vector<Jet>& jets, std::vector<Particle> const& Particles, Tag tag) const
{
    INFO0;
    for (auto const & Particle : Particles) {
        SortedByMinDeltaRTo(jets, Particle);
        if (jets.front().DeltaRTo(Particle) < 0.4_rad) jets.front().Info().SetTag(Tag::signal);
    }
    std::vector<Jet> NewCleanJets;
    for (auto const & Jet : jets) {

//         if (std::abs(Jet.rap()) > 2.5) continue;
// if (Jet.m() < 0){
//   ERROR("Clean Jets", "Massless Jet");
//           continue;
//         }
//         if (Tag == Tag::signal && Jet.Info().SumDisplacement() == 0) continue;
        if (Jet.Info().Tag() != tag) {
//   ERROR("Clean Jets", "Not Tagged Jet");
            continue;
        }
        NewCleanJets.emplace_back(Jet);
    }
    return NewCleanJets;
}

std::vector<Singlet> Tau::Multiplets(Event const& event, boca::PreCuts const&, TMVA::Reader const& reader)
{
    std::vector<Singlet> final_jets;
    INFO0;
    auto jets = event.Jets();
    for (auto & jet : jets) {
        jet.Info().SetBdt(Bdt(jet, reader));
        final_jets.emplace_back(Singlet(jet));
    }
    return final_jets;
}
std::string Tau::Name() const
{
    return "Tau";
}

}

}

}
