/**
 * Copyright (C) 2015-2016 Jan Hajer
 */
#pragma once

#include <functional>

#include "boca/multiplets/Quintet.hh"

#include "standardmodel/tagger/TopLeptonic.hh"
#include "standardmodel/tagger/Higgs.hh"

#include "toppartner/tagger/TopPartnerHadronicNeutral.hh"
#include "toppartner/branch/VetoTopPartner.hh"

namespace toppartner
{

namespace tagger
{

/**
 * @brief Semi leptonic heavy higgs BDT tagger
 *
 */
class VetoTopPartnerLeptonic : public Tagger<Quintet, branch::VetoTopPartnerBranch>
{

public:

    int Train(boca::Event const& event, PreCuts const& pre_cuts, Tag tag) override;

    std::vector<Quintet> Multiplets(boca::Event const& event, PreCuts const& pre_cuts, TMVA::Reader const& reader) override;

    std::string Name() const override;

    latex::String LatexName() const override;

private:

    std::vector<Particle> Particles(boca::Event const& event) const;

    std::vector<Quintet> Quintets(boca::Event const& event, std::function<Quintet(Quintet&)> const& function);

    Reader<TopPartnerHadronicNeutral> partner_reader_;

    Reader<standardmodel::tagger::TopLeptonic> top_reader_;

    Reader<standardmodel::tagger::Higgs> higgs_reader_;

};

}

}
