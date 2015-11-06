/**
 * Copyright (C) 2015 Jan Hajer
 */
#include "fastjet/tools/MassDropTagger.hh"
#include "fastjet/tools/Filter.hh"

#include "Doublet.hh"
#include "Event.hh"
#include "Types.hh"
#include "JetInfo.hh"
#include "InfoRecombiner.hh"
#include "HiggsTagger.hh"
#include "MomentumRange.hh"
#include "Exception.hh"
#include "Debug.hh"

namespace boca
{

namespace standardmodel
{

HiggsTagger::HiggsTagger()
{
    Info0;
    higgs_mass_window = 70. * GeV;
}

int HiggsTagger::Train(Event const& event, PreCuts const& pre_cuts, Tag tag) const
{
    Info0;
    Jets leptons = event.Leptons().leptons();
    return SaveEntries(Doublets(event, [&](Doublet & doublet) {
        return CheckDoublet(doublet, leptons, pre_cuts, tag);
    }), Particles(event), tag, Id::higgs);
}

std::vector<Doublet> HiggsTagger::Doublets(Event const& event, std::function<Doublet(Doublet&)> const& function) const
{
    Info0;
    Jets jets =  event.Hadrons().Jets();
    MomentumRange jet_range(Id::higgs, Id::higgs);
    std::vector<Doublet> doublets = unordered_pairs(jet_range.SofterThanMax(jets), [&](fastjet::PseudoJet const & jet_1, fastjet::PseudoJet const & jet_2) {
        Doublet doublet(jet_1, jet_2);
        if (!jet_range.BelowUpperBound(doublet)) throw boca::Problematic();
        return function(doublet);
    });
    for (auto const & jet : jet_range.HarderThanMin(jets)) {
        MomentumRange sub_jet_range((SubJet(Id::higgs)), (SubJet(Id::higgs)));
        if (sub_jet_range.BelowUpperBound(jet)) try {
                Jets pieces = Tagger::SubJets(jet, 2);
                Doublet doublet(pieces.at(0), pieces.at(1));
                doublets.emplace_back(function(doublet));
            } catch (std::exception const&) {}
        if (sub_jet_range.AboveLowerBound(jet)) try {
                Doublet doublet(jet);
                doublets.emplace_back(function(doublet));
            } catch (std::exception const&) {}
    }
    return doublets;
}

Jets HiggsTagger::Particles(Event const& event) const
{
    Info0;
    return CopyIfParticles(event.Partons().GenParticles(), Id::higgs, Id::CP_violating_higgs);
}

Doublet HiggsTagger::CheckDoublet(Doublet& doublet, Jets& leptons, PreCuts const& pre_cuts, Tag tag) const
{
    Info0;
    doublet = PrepareDoublet(doublet, leptons);
    if (Problematic(doublet, pre_cuts, tag)) throw boca::Problematic();
    doublet.SetTag(tag);
    return doublet;
}

Doublet HiggsTagger::PrepareDoublet(Doublet& doublet, Jets& leptons) const
{
    Info0;
    //     doublet = MassDrop(doublet);
    doublet = Doublet(bottom_reader_.Multiplet(doublet.Singlet1().Jet()), bottom_reader_.Multiplet(doublet.Singlet2().Jet()));
    SetClosestLepton(doublet, leptons);
    return doublet;
}

bool HiggsTagger::Problematic(Doublet const& doublet, PreCuts const& pre_cuts, Tag tag) const
{
    Info0;
    if (Problematic(doublet, pre_cuts)) return true;
    switch (tag) {
    case Tag::signal :
        if (pre_cuts.NotParticleRho(doublet)) return true;
        if (boost::units::abs(doublet.Mass() - MassOf(Id::higgs)) > higgs_mass_window) return true;
//         if (doublet.Singlet1().Bdt() < 0 || doublet.Singlet2().Bdt() < 0) return true;
        break;
    case Tag::background :
        break;
    }
    return false;
}

bool HiggsTagger::Problematic(Doublet const& doublet, PreCuts const& pre_cuts) const
{
    Info0;
    if (pre_cuts.ApplyCuts(Id::higgs, doublet)) return true;
    return false;
}

std::vector<Doublet> HiggsTagger::Multiplets(Event const& event, PreCuts const& pre_cuts, TMVA::Reader const& reader) const
{
    Info0;
    Jets leptons = event.Leptons().leptons();
    return ReduceResult(Doublets(event, [&](Doublet & doublet) {
        return Multiplet(doublet, leptons, pre_cuts, reader);
    }));
}

Doublet HiggsTagger::Multiplet(Doublet& doublet, Jets& leptons, PreCuts const& pre_cuts, TMVA::Reader const& reader) const
{
    Info0;
    doublet = PrepareDoublet(doublet, leptons);
    if (Problematic(doublet, pre_cuts)) throw boca::Problematic();
    doublet.SetBdt(Bdt(doublet, reader));
    return doublet;
}

Doublet HiggsTagger::MassDrop(Doublet const& doublet) const
{
    Info0;
    InfoRecombiner info_recombiner;
    fastjet::JetDefinition jet_definition(fastjet::cambridge_algorithm, doublet.DeltaR() + to_float(DetectorGeometry::JetConeSize()) * 2, &info_recombiner);
    fastjet::ClusterSequence& cluster_sequence = *new fastjet::ClusterSequence(doublet.Jet().constituents(), jet_definition);
    unsigned jet_number = 1;
    Jets exclusive_jets = cluster_sequence.exclusive_jets(int(jet_number));
    Check(exclusive_jets.size() == jet_number, jet_number);
    if (exclusive_jets.empty()) {
        delete &cluster_sequence;
        throw Empty();
    }
    cluster_sequence.delete_self_when_unused();

    fastjet::MassDropTagger mass_drop_tagger(0.667, 0.09);
    fastjet::PseudoJet mass_drop_jet = mass_drop_tagger(exclusive_jets.front());
    if (mass_drop_jet == 0) throw Empty();

    double radius = mass_drop_jet.pieces().at(0).delta_R(mass_drop_jet.pieces().at(1));
    radius = std::min(radius / 2, .3);
    fastjet::Filter filter(fastjet::JetDefinition(fastjet::cambridge_algorithm, radius, &info_recombiner), fastjet::SelectorNHardest(3));
    fastjet::PseudoJet filtered_jet = filter.result(mass_drop_jet);
    if (!filtered_jet.has_pieces()) throw Empty();
    Jets pieces = fastjet::sorted_by_pt(filtered_jet.pieces());
    if (pieces.size() < 2) throw Empty();
    return Doublet(bottom_reader_.Multiplet(pieces.at(0)), bottom_reader_.Multiplet(pieces.at(1)));
}

std::string HiggsTagger::Name() const
{
    return "Higgs";
}

std::string HiggsTagger::NiceName() const
{
    return "h";
}

}

}