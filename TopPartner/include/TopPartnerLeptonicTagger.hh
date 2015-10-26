#pragma once

#include "TopLeptonicTagger.hh"
#include "BosonTagger.hh"
#include "Quintet.hh"
#include "BranchesTopPartner.hh"

namespace boca
{

namespace naturalness
{

/**
 * @brief Semi leptonic heavy higgs BDT tagger
 *
 */
class TopPartnerLeptonicTagger : public TaggerTemplate<Quintet, TopPartnerBranch>
{

public:

    int Train(Event const& event, PreCuts const& pre_cuts, Tag tag) const final;

    std::vector<Quintet> Multiplets(Event const& event, PreCuts const& pre_cuts, TMVA::Reader const& reader) const final;

    std::string Name() const final;

    std::string NiceName() const final;

    Jets Particles(Event const& event) const;

private:

    std::vector<Quintet> Quintets(Event const& event, std::function<Quintet(Quintet&)> const& function) const;

    Reader<TopLeptonicTagger> top_reader_;

    Reader<BosonTagger> boson_reader_;
};

}

}
