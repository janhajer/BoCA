#pragma once

#include "standardmodel/tagger/Higgs.hh"
#include "boca/multiplets/Octet.hh"

#include "higgscpv/MultipletSignature.hh"
#include "higgscpv/tagger/TopLeptonicPair.hh"
#include "higgscpv/branch/SignatureLeptonic.hh"

namespace higgscpv
{

namespace tagger
{

/**
 *
 * @brief event BDT for
 *
 */
class Signature : public Tagger<boca::MultipletSignature< boca::Octet62 >, branch::SignatureLeptonic>
{

public:

    int Train(boca::Event const& event, boca::PreCuts const&, Tag tag) override;

    std::vector< boca::MultipletSignature< boca::Octet62 > > Multiplets(boca::Event const& event, boca::PreCuts const&, TMVA::Reader const& reader) override;

    std::string Name() const override;

private:

    Reader<standardmodel::tagger::Higgs> higgs_reader_;

    Reader<TopLeptonicPair> triplet_pair_reader_;

};

}

}
