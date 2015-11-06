/**
 * Copyright (C) 2015 Jan Hajer
 */
#include <boost/range/algorithm/adjacent_find.hpp>

#include "WHadronicTagger.hh"
#include "ParticleInfo.hh"
#include "Event.hh"
#include "Math.hh"
#include "Exception.hh"
// #define NOTIFICATION
#include "Debug.hh"

namespace boca
{

namespace standardmodel
{

WHadronicTagger::WHadronicTagger()
{
    Info0;
    w_mass_window_ = 40. * GeV;
}

int WHadronicTagger::Train(Event const& event, boca::PreCuts const& pre_cuts, Tag tag) const
{
    Info0;
    Jets jets = bottom_reader_.Jets(event);
    Info(jets.size());

    Info("2 jets form one W");
    std::vector<Doublet> doublets = Doublets(jets, pre_cuts, tag);

    for (auto const & jet : jets) {

        Info("1 jet forms one W");
        try {
            doublets.emplace_back(CheckDoublet(Doublet(jet), pre_cuts, tag));
        } catch (std::exception const&) {}

        Info("2 of 2 sub jets form one W");
        Jets pieces = bottom_reader_.SubMultiplet(jet, 2);
        try {
            doublets.emplace_back(CheckDoublet(Doublet(pieces.at(0), pieces.at(1)), pre_cuts, tag));
        } catch (std::exception const&) {}

        Info("1 of 2 sub jets forms one W");
        for (auto const & piece : pieces) {
            try {
                doublets.emplace_back(CheckDoublet(Doublet(piece), pre_cuts, tag));
            } catch (std::exception const&) {}
        }

        Info("2 of 3 sub jets forms one W");
        pieces = bottom_reader_.SubMultiplet(jet, 3);
        doublets = Join(doublets, Doublets(pieces, pre_cuts, tag));

    }

    Jets w_particles = Particles(event);
    Info(doublets.size(), w_particles.size());
    return SaveEntries(BestMatches(doublets, w_particles, tag, Id::W));
}

Jets WHadronicTagger::Particles(Event const& event) const
{
    Jets particles = event.Partons().GenParticles();
    Jets quarks = CopyIfMother(CopyIfQuark(particles), Id::W);
    if (quarks.empty()) return {};
    std::vector<int> ids;
    for (auto const & quark : quarks) ids.emplace_back(quark.user_info<ParticleInfo>().Family().mother_1().id());
    if (boost::range::adjacent_find(ids, std::not_equal_to<int>()) == ids.end()) return CopyIfExactParticle(particles, ids.front());
    else return CopyIfParticle(particles, Id::W);
}

Doublet WHadronicTagger::CheckDoublet(Doublet doublet, PreCuts const& pre_cuts, Tag tag) const
{
    if (Problematic(doublet, pre_cuts, tag)) throw boca::Problematic();
    doublet.SetTag(tag);
    return doublet;
}

std::vector<Doublet> WHadronicTagger::Doublets(boca::Jets const& jets, boca::PreCuts const& pre_cuts, boca::Tag tag) const
{
    return unordered_pairs(jets, [&](fastjet::PseudoJet const & jet_1, fastjet::PseudoJet const & jet_2) {
        Doublet doublet(jet_1, jet_2);
        if (Problematic(doublet, pre_cuts, tag)) throw boca::Problematic();
        return doublet;
    });
}

bool WHadronicTagger::Problematic(Doublet const& doublet, PreCuts const& pre_cuts, Tag tag) const
{
    if (Problematic(doublet, pre_cuts))
        return true;
    switch (tag) {
    case Tag::signal :
        if (pre_cuts.OutSideMassWindow(doublet, w_mass_window_, Id::W)) return true;
        if (pre_cuts.NotParticleRho(doublet)) return true;
//         if (doublet.Singlet1().Bdt() > 1) return true;
//         if (doublet.Singlet2().Bdt() > 1) return true;
        break;
    case Tag::background :
        break;
    }
    return false;
}

bool WHadronicTagger::Problematic(Doublet const& doublet, PreCuts const& pre_cuts) const
{
    if (pre_cuts.ApplyCuts(Id::W, doublet)) return true;
    if (double(doublet.DeltaR()) * rad < DetectorGeometry::MinCellResolution() && doublet.DeltaR() > 0) return true;
    return false;
}

std::vector<Doublet> WHadronicTagger::Multiplets(Event const& event, PreCuts const& pre_cuts, TMVA::Reader const& reader) const
{
    Info0;
    Jets jets = bottom_reader_.Jets(event);
    std::vector<Doublet> doublets;
    Info("2 jets form one W");
    doublets = Join(doublets, Multiplets(jets, pre_cuts, reader));
    Info("2 of 2 sub jets form one W");
    doublets = Join(doublets, SubMultiplets(jets, pre_cuts, reader, 2));
    Info("2 of 3 sub jets forms one W");
    doublets = Join(doublets, SubMultiplets(jets, pre_cuts, reader, 3));
    Info("1 of 2 sub jets form one W");
    doublets = Join(doublets, SubMultiplets2(jets, pre_cuts, reader));
    Info("1 jets form one W");
    doublets = Join(doublets, Multiplets3(jets, pre_cuts, reader));
    return ReduceResult(doublets);
}

std::vector<Doublet> WHadronicTagger::Multiplets(Jets const& jets, PreCuts const& pre_cuts, TMVA::Reader const& reader) const
{
    Info0;
    std::vector<Doublet>  doublets = unordered_pairs(jets, [&](fastjet::PseudoJet const & jet_1, fastjet::PseudoJet const & jet_2) {
        return Multiplet(jet_1, jet_2, pre_cuts, reader);
    });
    return doublets;
}

std::vector<Doublet> WHadronicTagger::Multiplets3(Jets const& jets, PreCuts const& pre_cuts, TMVA::Reader const& reader) const
{
    Info0;
    std::vector<Doublet>  doublets;
    for (auto const & jet : jets) {
        try {
            doublets.emplace_back(Multiplet(jet, pre_cuts, reader));
        } catch (std::exception const&) {}
    }
    return doublets;
}

std::vector<Doublet> WHadronicTagger::SubMultiplets(Jets const& jets, PreCuts const& pre_cuts, TMVA::Reader const& reader, size_t sub_jet_number) const
{
    Info0;
    std::vector<Doublet>  doublets;
    for (auto const & jet : jets) {
        Jets pieces = bottom_reader_.SubMultiplet(jet, sub_jet_number);
        if (pieces.size() < sub_jet_number) continue;
        doublets = Join(doublets, unordered_pairs(pieces, [&](fastjet::PseudoJet const & piece_1, fastjet::PseudoJet const & piece_2) {
            return Multiplet(piece_1, piece_2, pre_cuts, reader);
        }));
    }
    return doublets;
}

std::vector<Doublet> WHadronicTagger::SubMultiplets2(Jets const& jets, PreCuts const& pre_cuts, TMVA::Reader const& reader) const
{
    Info0;
    size_t sub_jet_number = 2;
    std::vector<Doublet>  doublets;
    for (auto const & jet : jets) {
        Jets pieces = bottom_reader_.SubMultiplet(jet, sub_jet_number);
        for (auto const & piece : pieces) {
            try {
                doublets.emplace_back(Multiplet(piece, pre_cuts, reader));
            }  catch (std::exception const&) {}
        }
    }
    return doublets;
}

Doublet WHadronicTagger::SubMultiplet(fastjet::PseudoJet const& jet, PreCuts const& pre_cuts, TMVA::Reader const& reader) const
{
    Info0;
    Jets pieces = bottom_reader_.SubMultiplet(jet, 2);
    Doublet doublet;
    if (pieces.empty()) throw Empty();
    if (pieces.size() == 1) doublet.SetJet(pieces.front());
    else doublet.SetMultiplets(pieces.at(0), pieces.at(1));
    try {
        return Multiplet(doublet, pre_cuts, reader);
    } catch (std::exception const&) {
        throw;
    }
}

Doublet WHadronicTagger::Multiplet(fastjet::PseudoJet const& jet_1, fastjet::PseudoJet const& jet_2, PreCuts const& pre_cuts, TMVA::Reader const& reader) const
{
    Info0;
    Doublet doublet(jet_1, jet_2);
    try {
        return Multiplet(doublet, pre_cuts, reader);
    } catch (std::exception const&) {
        throw;
    }
}

Doublet WHadronicTagger::Multiplet(fastjet::PseudoJet const& jet, PreCuts const& pre_cuts, TMVA::Reader const& reader) const
{
    Info0;
    Doublet doublet(jet);
    try {
        return Multiplet(doublet, pre_cuts, reader);
    } catch (std::exception const&) {
        throw;
    }
}

Doublet WHadronicTagger::Multiplet(Doublet& doublet, PreCuts const& pre_cuts, TMVA::Reader const& reader) const
{
    Info0;
    if (Problematic(doublet, pre_cuts)) throw boca::Problematic();
    doublet.SetBdt(Bdt(doublet, reader));
    return doublet;
}

Doublet WHadronicTagger::Multiplet(fastjet::PseudoJet const& jet, TMVA::Reader const& reader) const
{
    PreCuts pre_cuts;
    try {
        return Multiplet(jet, pre_cuts, reader);
    } catch (std::exception const&) {
        throw;
    }
}

Doublet WHadronicTagger::SubMultiplet(fastjet::PseudoJet const& jet, TMVA::Reader const& reader) const
{
    PreCuts pre_cuts;
    try {
        return SubMultiplet(jet, pre_cuts, reader);
    } catch (std::exception const&) {
        throw;
    }
}

Doublet WHadronicTagger::Multiplet(fastjet::PseudoJet const& jet_1, fastjet::PseudoJet const& jet_2, TMVA::Reader const& reader) const
{
    PreCuts pre_cuts;
    try {
        return Multiplet(jet_1, jet_2, pre_cuts, reader);
    } catch (std::exception const&) {
        throw;
    }
}

std::string WHadronicTagger::Name() const
{
    return "WHadronic";
}

std::string WHadronicTagger::NiceName() const
{
    return "W_{h}";
}

}

}
