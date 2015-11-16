/**
 * Copyright (C) 2015 Jan Hajer
 */
#pragma once

#include <functional>
#include "Decuplet.hh"
#include "VetoTopPartnerHadronicTagger.hh"

namespace boca
{

namespace naturalness
{

/**
 * @brief Semi leptonic heavy higgs BDT tagger
 *
 */
class SignatureSingleLeptonicTagger : public TaggerTemplate<Decuplet532, SignatureSingleBranch>
{

public:

    int Train(Event const& event, PreCuts const& pre_cuts, Tag tag) const final;

    std::vector<Decuplet532> Multiplets(Event const& event, PreCuts const& pre_cuts, TMVA::Reader const& reader) const final;

    std::string Name() const final;

    std::string NiceName() const final;

private:

    std::vector<Decuplet532> Decuplets(Event const& event, std::function<Decuplet532(Decuplet532&)> const& function) const;

    Reader<TopPartnerLeptonicTagger> partner_reader_;

    Reader<standardmodel::TopHadronicTagger> top_reader_;

    Reader<standardmodel::HiggsTagger> higgs_reader_;

    Reader<VetoTopPartnerHadronicTagger> veto_reader_;
};

}

}
