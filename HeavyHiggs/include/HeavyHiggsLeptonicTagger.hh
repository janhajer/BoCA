#pragma once

#include "TopLeptonicTagger.hh"
#include "Quartet.hh"
#include "Sextet.hh"
#include "Branch.hh"

namespace analysis
{

namespace heavyhiggs
{

/**
 * @brief Leptonic heavy higgs BDT tagger
 *
 */
class HeavyHiggsLeptonicTagger : public BranchTagger<HeavyHiggsLeptonicBranch>
{

public:

    HeavyHiggsLeptonicTagger();

    int Train(Event &event, const Tag tag);

    std::vector<Sextet> Multiplets(Event &event, const TMVA::Reader &reader);

private:

    TopLeptonicTagger top_leptonic_tagger_;

    Reader top_leptonic_reader_;

};

}

}
