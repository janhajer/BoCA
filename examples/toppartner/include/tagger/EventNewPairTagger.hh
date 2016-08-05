#pragma once

#include "boca/MultipletEvent.hh"
#include "boca/analysis/AnalysisBase.hh"
#include "boca/branch/Event.hh"

#include "include/tagger/NewPartnerPairTagger.hh"

namespace toppartner
{

/**
 *
 * @brief Prepares multivariant analysis
 *
 */
class EventNewPairTagger : public Tagger<MultipletEvent<Decuplet55>, boca::branch::Event>
{

public:

    int Train(boca::Event const& event, PreCuts const& pre_cuts, Tag tag) override;

    std::vector<MultipletEvent<Decuplet55>> Multiplets(boca::Event const& event, PreCuts const& pre_cuts, TMVA::Reader const& reader) override;

    std::string Name() const override;

    latex::String LatexName() const override;

private:

    Reader<NewPartnerPairTagger> signature_reader_;

    Reader<standardmodel::tagger::Bottom> bottom_reader_;

};

}