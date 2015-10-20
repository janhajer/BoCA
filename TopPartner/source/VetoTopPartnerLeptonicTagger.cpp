/**
 * Copyright (C) 2015 Jan Hajer
 */
#include "VetoTopPartnerLeptonicTagger.hh"
#include "Decuplet.hh"
#include "ParticleInfo.hh"
// #define DEBUG
// #define INFORMATION
#include "Debug.hh"

namespace boca
{

namespace naturalness
{

int VetoTopPartnerLeptonicTagger::Train(Event const& event, PreCuts const& pre_cuts, Tag tag) const
{
    Info();
    return SaveEntries(Quintets(event, pre_cuts, [&](Quintet & quintet) {
        quintet.SetTag(tag);
        return quintet;
    }),Particles(event), tag);
}

std::vector<Quintet> VetoTopPartnerLeptonicTagger::Multiplets(Event const& event, boca::PreCuts const& pre_cuts, TMVA::Reader const& reader) const
{
    Info();
    return ReduceResult(Quintets(event, pre_cuts, [&](Quintet & quintet) {
        quintet.SetBdt(Bdt(quintet, reader));
        return quintet;
    }));
}

Jets VetoTopPartnerLeptonicTagger::Particles(Event const& event) const
{
  Jets particles = event.Partons().GenParticles();
  Jets leptons = CopyIfLepton(particles);
  Jets candidate = CopyIfGrandGrandMother(leptons, Id::top_partner);
  if (!candidate.empty()) {
    int grand_grand_mother = candidate.front().user_info<ParticleInfo>().Family().grand_grand_mother().id();
    return CopyIfExactParticle(particles, grand_grand_mother);
  } else {
    candidate = CopyIfGrandMother(leptons, Id::top_partner);
    candidate = CopyIfMother(candidate, Id::W);
    if (candidate.empty()) return {};
    int grand_mother = candidate.front().user_info<ParticleInfo>().Family().grand_mother().id();
    return CopyIfExactParticle(particles, grand_mother);
  }
}

// std::vector<Quintet> VetoTopPartnerLeptonicTagger::Quintets(Event const& event, PreCuts const& pre_cuts, std::function<Quintet(Quintet&)> const& function) const
// {
//     Info();
//     return triples(partner_reader_.Multiplets(event), top_reader_.Multiplets(event), higgs_reader_.Multiplets(event), [&](Quintet const & quintet, Triplet const& triplet, Doublet const & doublet) {
//         Decuplet532 decuplet(quintet, triplet, doublet);
//         if (decuplet.Overlap()) throw Overlap();
//         Quintet veto(triplet, doublet);
//         return function(veto);
//     });
// }

std::vector<Quintet> VetoTopPartnerLeptonicTagger::Quintets(Event const& event, PreCuts const& pre_cuts, std::function<Quintet(Quintet&)> const& function) const
{
    Info();
    std::vector<Quintet> quintets;
    for (auto const & doublet : higgs_reader_.Multiplets(event)) {
        for (auto const & triplet : top_reader_.Multiplets(event)) {
            Quintet veto(triplet, doublet);
            if (veto.Overlap()) continue;
            for (auto const & quintet : partner_reader_.Multiplets(event)) {
                Decuplet532 decuplet(quintet, triplet, doublet);
                if (decuplet.Overlap()) continue;
                quintets.emplace_back(function(veto));
                break;
            }
        }
    }
    return quintets;
}

}

}