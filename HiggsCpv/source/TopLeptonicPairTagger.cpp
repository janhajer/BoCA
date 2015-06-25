# include "TopLeptonicPairTagger.hh"
# include "Quartet22.hh"
# include "WimpMass.hh"
# include "Predicate.hh"

namespace analysis
{

namespace higgscpv
{

TopLeptonicPairTagger::TopLeptonicPairTagger()
{
//     debug_level_ = Severity::Debug;
    Print(Severity::Notification, "Constructor");
    set_tagger_name("TopLeptonicPair");
    top_leptonic_reader_.SetTagger(top_leptonic_tagger_);
    DefineVariables();
}

int TopLeptonicPairTagger::Train(Event &event, PreCuts &, const Tag tag)
{
    Print(Severity::Information, "Top Leptonic Pair Tagger Tags");
    std::vector<Doublet> doublets = top_leptonic_reader_.Multiplets<TopLeptonicTagger>(event);
    Print(Severity::Debug, "Number of Doublets", doublets.size());

    Jets particles = event.Partons().GenParticles();
    Jets top_particles = copy_if_abs_particle(particles, Id::Top);
    Jets neutrinos = copy_if_neutrino(particles);
    if (top_particles.size() != 2 && tag == Tag::Signal) Print(Severity::Error, "Number of Tops?", particles.size());

    std::vector<Doublet> final_doublets;
    switch (tag) {
    case Tag::Signal :
        for (const auto & doublet : doublets) for (const auto top : top_particles) if (doublet.Coincides(top)) final_doublets.emplace_back(doublet);
        break;
    case Tag::Background :
        final_doublets = doublets;
    }
    Print(Severity::Debug, "Number of Doublets", final_doublets.size());

    std::vector<Sextet> sextets;
    for (auto doublet1 = doublets.begin(); doublet1 != doublets.end(); ++doublet1)
        for (auto doublet2 = doublet1 + 1; doublet2 != doublets.end(); ++doublet2) {
            Quartet22 quartet(*doublet1, *doublet2);
            if (quartet.Overlap()) continue;
            WimpMass wimp_mass;
//             sextets = Join(sextets, wimp_mass.Sextet(quartet, event.Hadrons().MissingEt(), neutrinos, tag));
            sextets.emplace_back(wimp_mass.Fake(quartet));
        }
    Print(Severity::Debug, "Number of Sextets", sextets.size());
    if (tag == Tag::Signal) sextets = BestRapidity(sextets);
    return SaveEntries(sextets);
}

std::vector< Sextet > TopLeptonicPairTagger::Multiplets(Event &event, PreCuts &, const TMVA::Reader &reader)
{
    std::vector<Doublet> doublets = top_leptonic_reader_.Multiplets<TopLeptonicTagger>(event);
    Print(Severity::Information, "Doublets", doublets.size());
    std::vector<Sextet>  sextets;
    for (auto doublet1 = doublets.begin(); doublet1 != doublets.end(); ++doublet1)
        for (auto doublet2 = doublet1 + 1; doublet2 != doublets.end(); ++doublet2) {
            Quartet22 quartet(*doublet1, *doublet2);
            if (quartet.Overlap()) continue;
            Print(Severity::Information, "Quartet");
            WimpMass wimp_mass;
//             for (auto sextet : wimp_mass.Sextets(quartet, event.Hadrons().MissingEt())) {
                Sextet sextet = wimp_mass.Fake(quartet);
                Print(Severity::Information, "Sextet");
                sextet.SetBdt(Bdt(sextet,reader));
                sextets.emplace_back(sextet);
//             }
        }
    Print(Severity::Information, "Sextets", sextets.size());
    return ReduceResult(sextets);
}

}

}
