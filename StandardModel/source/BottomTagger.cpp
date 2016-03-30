/**
 * Copyright (C) 2015-2016 Jan Hajer
 */
#include "BottomTagger.hh"

#include "Event.hh"
#include "Particles.hh"
#include "PreCuts.hh"
#include "Exception.hh"
// #define DEBUGGING
#include "DEBUG.hh"

namespace boca
{

namespace standardmodel
{

BottomTagger::BottomTagger()
{
    INFO0;
    bottom_max_mass_ = 75_GeV;
}

int BottomTagger::Train(Event const& event, PreCuts const& pre_cuts, Tag tag) const
{
    INFO0;
    auto jets = Jets(event, pre_cuts, [&](Jet & jet) {
        if (Problematic(jet, pre_cuts, tag)) throw boca::Problematic();
        return jet;
    });
    return SaveEntries(jets, Particles(event), tag);
}

std::vector<Particle> BottomTagger::Particles(Event const& event) const
{
    INFO0;
    return RemoveIfSoft(CopyIfParticle(event.Partons().Particles(), Id::bottom), DetectorGeometry::JetMinPt());
}

std::vector<Jet> BottomTagger::Jets(Event const& event, PreCuts const& pre_cuts, std::function<Jet(Jet&)> const& function) const
{
    INFO0;
    std::vector<Jet> jets = event.Hadrons().Jets();
    INFO(jets.size());
    std::vector<Jet> bottoms = Multiplets(jets, function);
    if (pre_cuts.DoSubJets(Id::bottom)) {
        Insert(bottoms, Multiplets(jets, function, 2));
        Insert(bottoms, Multiplets(jets, function, 3));
    }
    INFO(bottoms.size());
    return bottoms;
}

std::vector<Jet> BottomTagger::Multiplets(std::vector<Jet> jets, std::function<Jet(Jet&)> const& function, unsigned sub_jet_number) const
{
    INFO0;
    if (sub_jet_number > 1) jets = SubJets(jets, sub_jet_number);
    std::vector<Jet> final_jets;
    for (auto & jet : jets) try {
      DEBUG(jet.m(),jet.rap(),jet.phi_std(),jet.has_user_info());
            final_jets.emplace_back(function(jet));
        } catch (std::exception&) {
            continue;
        }
    return final_jets;
}

std::vector<Jet> BottomTagger::Multiplets(Event const& event, PreCuts const& pre_cuts, TMVA::Reader const& reader) const
{
    INFO0;
    return Jets(event, pre_cuts, [&](Jet & jet) {
        if (Problematic(jet, pre_cuts)) throw boca::Problematic();
        return Multiplet(jet, reader);
    });
}

Jet BottomTagger::Multiplet(Jet & jet, TMVA::Reader const& reader) const
{
    INFO0;
    DEBUG(jet.m(),jet.rap(),jet.phi_std(),jet.has_user_info());
    jet.Info().SetBdt(Bdt(jet, reader));
    return jet;
}

bool BottomTagger::Problematic(Jet const& jet, PreCuts const& pre_cuts, Tag tag) const
{
    INFO0;
    if (Problematic(jet, pre_cuts)) return true;
    if (jet.Mass() > bottom_max_mass_) return true;
    if (boost::units::abs(jet.Rap()) > DetectorGeometry::TrackerEtaMax()) return true;
    switch (tag) {
    case Tag::signal :
        if (jet.Info().SumDisplacement() == 0_mm) return true;
        break;
    case Tag::background : break;
    }
    return false;
}

bool BottomTagger::Problematic(Jet const& jet, PreCuts const& pre_cuts) const
{
    INFO0;
    if (pre_cuts.ApplyCuts(Id::bottom, jet)) return true;
    return false;
}

std::vector<Jet> BottomTagger::SubJets(std::vector<Jet> const& jets, int sub_jet_number) const
{
    INFO0;
    std::vector<Jet> subjets;
    for (auto const & jet : jets) Insert(subjets, Tagger::SubJets(jet, sub_jet_number));
    return subjets;
}

std::vector<Jet> BottomTagger::Jets(Event const& event, boca::PreCuts const& pre_cuts, TMVA::Reader const& reader) const
{
    INFO0;
    return Multiplets(event.Hadrons().Jets(), [&](Jet & jet) {
        if (Problematic(jet, pre_cuts)) throw boca::Problematic();
        return Multiplet(jet, reader);
    });
}

std::vector<Jet> BottomTagger::SubMultiplet(Jet const& jet, TMVA::Reader const& reader, int sub_jet_number) const
{
    INFO0;
    std::vector<Jet> jets;
    for (auto & sub_jet : Tagger::SubJets(jet, sub_jet_number)) {
        if (sub_jet.Mass() <= massless) continue;
        jets.emplace_back(Multiplet(sub_jet, reader));
    }
    return jets;
}

std::string BottomTagger::Name() const
{
    return "Bottom";
}

std::string BottomTagger::LatexName() const
{
    return "b";
}

}

}
