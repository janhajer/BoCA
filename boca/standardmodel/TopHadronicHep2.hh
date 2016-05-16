#pragma once

#include "boca/TaggerTemplate.hh"
#include "boca/multiplets/Triplet.hh"
#include "boca/standardmodel/Branches.hh"

namespace boca
{

namespace standardmodel
{

/**
 * @brief BDT tagger for hadronically decaying top quarks based on the HEP top tagger
 *
 */
class TopHadronicHep2 : public TaggerTemplate<Triplet, TopHadronicHepBranch>
{

public:

    TopHadronicHep2();

    int Train(Event const& event, PreCuts const& pre_cuts, Tag tag) override;

    using TaggerTemplate::Multiplets;

    std::vector<Triplet> Multiplets(Event const& event, boca::PreCuts const& pre_cuts, TMVA::Reader const& reader) override;

    std::string Name() const override;

    latex::String LatexName() const override;

    /**
     * @brief tagges the tops
     *
     */
    std::vector<fastjet::PseudoJet> Tops(std::vector<fastjet::PseudoJet>& e_flows);

private:

    std::vector<Particle> Particles(boca::Event const& event) const;

    bool Problematic(Triplet const& triplet, PreCuts const& pre_cuts, Tag tag) const;

    bool Problematic(Triplet const& triplet, PreCuts const& pre_cuts) const;

    std::vector<Triplet> Triplets(Event const& event, PreCuts const& pre_cuts, std::function<Triplet(Triplet&)> const& function) const;

    Mass top_mass_window_;

};

}
}