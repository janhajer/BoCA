#pragma once

#include "boca/TopPartnerLeptonicChargedTagger.hh"
#include "boca/TopPartnerLeptonicNeutralTagger.hh"

namespace boca
{

namespace toppartner
{

/**
 * @brief Top partner to bottom and W BDT tagger
 *
 */
class TopPartnerLeptonicTagger : public Tagger<Quintet, TopPartnerBranch>
{

public:

    int Train(Event const& event, PreCuts const&, Tag tag) override;

    using Tagger::Multiplets;

    std::vector<Quintet> Multiplets(Event const&, boca::PreCuts const&, TMVA::Reader const&) const{
      return {};
    }

    std::vector<Quintet> Multiplets(const boca::Event& event, const boca::PreCuts&);

    std::vector<Quintet> Multiplets(Event const& event);

    std::vector<Particle> Particles(Event const& event) const;

    std::string Name() const override;

    latex::String LatexName() const override;

private:

    Reader<TopPartnerLeptonicChargedTagger> charged_;

    Reader<TopPartnerLeptonicNeutralTagger> neutral_;

};

}

}