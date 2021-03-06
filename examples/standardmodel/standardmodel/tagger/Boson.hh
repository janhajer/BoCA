/**
 * Copyright (C) 2015-2016 Jan Hajer
 */
#pragma once

#include "boca/tagger/Tagger.hh"
#include "boca/multiplets/Doublet.hh"
#include "boca/multivariant/Reader.hh"
#include "standardmodel/tagger/Bottom.hh"
#include "standardmodel/branch/ZHadronic.hh"

namespace boca
{
class Doublet;
}

namespace standardmodel
{

namespace tagger {

/**
 * @brief BDT tagger for W, Z and Higgs bosons.
 * @author Jan Hajer
 * @copyright Copyright (C) 2015-2016 Jan Hajer
 * @date 2015
 * @license GPL 3
 *
 */
class Boson : public Tagger<Doublet, branch::ZHadronic>
{

public:

    Boson();

    int Train(boca::Event const& event, PreCuts const& pre_cuts, Tag tag) override;

    using Tagger::Multiplets;

    std::vector<Doublet> Multiplets(boca::Event const& event, PreCuts const& pre_cuts, TMVA::Reader const& reader) override;

    std::string Name() const override;

    latex::String LatexName() const override;

    boost::optional<Doublet> Multiplet(boca::Doublet& doublet, const boca::PreCuts& pre_cuts, TMVA::Reader const& reader);

private:

    std::vector<Particle> Particles(boca::Event const& event) const;

    boost::optional<Doublet> CheckDoublet(Doublet doublet, PreCuts const& pre_cuts, Tag tag) const;

    bool Problematic(Doublet const& doublet, PreCuts const& pre_cuts, Tag tag) const;

    bool Problematic(Doublet const& doublet, PreCuts const& pre_cuts) const;

    std::vector<Doublet> Doublets(boca::Event const& event, std::function<boost::optional<Doublet>(Doublet&)> const& function);

    Reader<tagger::Bottom> bottom_reader_;

    Mass boson_mass_window;

};

}

}
