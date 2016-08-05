#pragma once

#include "boca/multiplets/Octet.hh"
#include "boca/standardmodel/tagger/TopHadronic.hh"

#include "include/tagger/TopPartnerLeptonicNeutralTagger.hh"

namespace toppartner {

/**
 * @brief Semi leptonic heavy higgs BDT tagger
 *
 */
class TopPartnerTopPairTagger : public Tagger<Octet53,boca::branch::Multi> {

public:

    int Train(boca::Event const& event, PreCuts const& pre_cuts, Tag tag) override;

    std::vector<Octet53> Multiplets(boca::Event const& event, PreCuts const& pre_cuts, TMVA::Reader const& reader) override;

    std::string Name() const override;

private:

    Reader<TopPartnerLeptonicNeutralTagger> top_partner_reader_;

    Reader<standardmodel::tagger::TopHadronic> top_reader_;
};

}