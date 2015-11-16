/**
 * Copyright (C) 2015 Jan Hajer
 */
#pragma once

#include "BottomTagger.hh"
#include "TaggerTemplate.hh"
#include "Doublet.hh"
#include "Reader.hh"

namespace boca
{

/**
 * @brief JetPair BDT tagger
 *
 */
class JetPairTagger : public TaggerTemplate<Doublet, JetPairBranch>
{

public:

    int Train(Event const& event, PreCuts const& pre_cuts, Tag tag) const final;

    std::vector<Doublet> Multiplets(Event const& event, PreCuts const& pre_cuts, TMVA::Reader const& reader) const final;

    std::string Name() const final;

    Jets BottomPair(Event const& event, Tag tag) const;

    bool CheckIfBadBottom(boca::Doublet const& doublet, Jets const& jets) const;

    Jets HiggsParticle(Event const& event, Tag tag) const;

private:

    Reader<standardmodel::BottomTagger> bottom_reader_;

};

}
