# include "TopLeptonicPairTagger.hh"
# include "Quartet22.hh"
# include "WimpMass.hh"
# include "Predicate.hh"

higgscpv::TopLeptonicPairTagger::TopLeptonicPairTagger()
{
    Print(kNotification, "Constructor");
    set_tagger_name("TripletLeptonicPair");
    top_leptonic_reader_.set_tagger(top_leptonic_tagger_);
    DefineVariables();
}

void higgscpv::TopLeptonicPairTagger::DefineVariables()
{
    Print(kNotification , "Define Variables");
    AddVariable(branch_.Mass, "Mass");
    AddVariable(branch_.Pt, "Pt");
    AddVariable(branch_.Rap, "Rap");
    AddVariable(branch_.Phi, "Phi");
    AddVariable(branch_.Ht, "Ht");

    AddVariable(branch_.DeltaM, "DeltaM");
    AddVariable(branch_.DeltaPt, "DeltaPt");
    AddVariable(branch_.DeltaPhi, "DeltaPhi");
    AddVariable(branch_.DeltaRap, "DeltaRap");
    AddVariable(branch_.DeltaR, "DeltaR");

    AddVariable(branch_.BottomMass, "BottomMass");
    AddVariable(branch_.BottomPt, "BottomPt");
    AddVariable(branch_.BottomRap, "BottomRap");
    AddVariable(branch_.BottomPhi, "BottomPhi");
    AddVariable(branch_.BottomBdt, "BottomBdt");

    AddVariable(branch_.TopMass, "TopMass");
    AddVariable(branch_.TopPt, "TopPt");
    AddVariable(branch_.TopRap, "TopRap");
    AddVariable(branch_.TopPhi, "TopPhi");
    AddVariable(branch_.TopBdt, "TopBdt");

    AddVariable(branch_.Bdt, "Bdt");
    AddSpectator(branch_.Tag, "Tag");
}

int higgscpv::TopLeptonicPairTagger::Train(analysis::Event &event, const Tag tag)
{
    Print(kInformation, "W Tags");
    std::vector<analysis::Doublet> doublets = top_leptonic_reader_.Multiplets<analysis::TopLeptonicTagger>(event);

    analysis::Jets particles = event.Partons().GenParticles();
    analysis::Jets top_particles = analysis::copy_if_abs_particle(particles, TopId);
    analysis::Jets neutrinos = analysis::copy_if_neutrino(particles);
    if (top_particles.size() != 2 && tag == kSignal) Print(kError, "Number of Tops?", particles.size());

    std::vector<analysis::Doublet> final_doublets;
    switch (tag) {
    case kSignal :
        for (const auto & doublet : doublets) for(const auto top : top_particles) if (doublet.Coincides(top)) final_doublets.emplace_back(doublet);
        break;
    case kBackground :
        final_doublets = doublets;
    }

    std::vector<analysis::Sextet> sextets;
    for (auto doublet1 = doublets.begin(); doublet1 != doublets.end(); ++doublet1)
        for (auto doublet2 = doublet1+1; doublet2 != doublets.end(); ++doublet2) {
            analysis::Quartet22 quartet(*doublet1, *doublet2);
            if(quartet.Overlap()) continue;
            analysis::WimpMass wimp_mass;
            analysis::JoinVectors(sextets, wimp_mass.Sextets(quartet,event.Hadrons().MissingEt()));
        }

    Print(kDebug, "Number of Jet Pairs", sextets.size());
    if (tag == kSignal && sextets.size() > 1) {
        sextets = analysis::SortByMaxDeltaRap(sextets);
        if (sextets.size() > 1)sextets.erase(sextets.begin() + 1, sextets.end());
    }
    return SaveEntries(sextets);
}

std::vector< analysis::Sextet > higgscpv::TopLeptonicPairTagger::Multiplets(analysis::Event& event, const TMVA::Reader& reader)
{
    std::vector<analysis::Doublet> doublets = top_leptonic_reader_.Multiplets<analysis::TopLeptonicTagger>(event);
    std::vector<analysis::Sextet>  sextets;
    for (auto doublet1 = doublets.begin(); doublet1 != doublets.end(); ++doublet1)
        for (auto doublet2 = doublet1+1; doublet2 != doublets.end(); ++doublet2) {
            analysis::Quartet22 quartet(*doublet1, *doublet2);
            if(quartet.Overlap()) continue;
            analysis::WimpMass wimp_mass;
            for(auto sextet : wimp_mass.Sextets(quartet,event.Hadrons().MissingEt())) {
                branch_ = branch(sextet);
                sextet.SetBdt(Bdt(reader));
                sextets.emplace_back(sextet);
            }
        }
    return ReduceResult(sextets);
}
