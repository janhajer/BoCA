# include "HChargedHiggsLeptonicTagger.hh"

hanalysis::HChargedHiggsLeptonicTagger::HChargedHiggsLeptonicTagger(const BottomTagger &NewBottomTagger, const HTopLeptonicTagger &NewTopLeptonicTagger)
{
//     DebugLevel = hanalysis::HObject::HDebug;

    Print(HNotification, "Constructor");

    bottom_tagger_ = NewBottomTagger;
    BottomReader.set_tagger(bottom_tagger_);
    TopLeptonicTagger = NewTopLeptonicTagger;
    TopLeptonicReader.set_tagger(TopLeptonicTagger);

    set_tagger_name("ChargedHiggsLeptonic");

    DefineVariables();
}

hanalysis::HChargedHiggsLeptonicTagger::~HChargedHiggsLeptonicTagger()
{
    Print(HNotification, "Destructor");
}

HChargedHiggsLeptonicBranch hanalysis::HChargedHiggsLeptonicTagger::GetBranch(const HTriplet &Triplet)
{
    Print(HInformation, "FillPairTagger", Triplet.Bdt());

    HChargedHiggsLeptonicBranch ChargedHiggsBranch;
    ChargedHiggsBranch.HeavyHiggsMass = Triplet.Jet().m();
    ChargedHiggsBranch.HeavyHiggsPt = Triplet.Jet().m();

    ChargedHiggsBranch.TopDeltaR = Triplet.DeltaR();
    ChargedHiggsBranch.TopDeltaRap = Triplet.DeltaRap();
    ChargedHiggsBranch.TopDeltaPhi = Triplet.DeltaPhi();

//     HeavyHiggsBranch->LargerWDeltaR = Quartet.GetLargerTripletDeltaR();
//     HeavyHiggsBranch->LargerWDeltaRap = Quartet.GetLargerTripletDeltaRap();
//     HeavyHiggsBranch->LargerWDeltaPhi = Quartet.GetLargerTripleDeltaPhi();
//
//     HeavyHiggsBranch->SmallerWDeltaR = Quartet.GetSmallerTripletDeltaR();
//     HeavyHiggsBranch->SmallerWDeltaRap = Quartet.GetSmallerTripletDeltaRap();
//     HeavyHiggsBranch->SmallerWDeltaPhi = Quartet.GetSmallerTripletDeltaPhi();
//
//     HeavyHiggsBranch->LargerNeutrinoDeltaR = Quartet.GetLargerTripletDeltaR();
//     HeavyHiggsBranch->LargerNeutrinoDeltaRap = Quartet.GetLargerTripletDeltaRap();
//     HeavyHiggsBranch->LargerNeutrinoDeltaPhi = Quartet.GetLargerTripleDeltaPhi();
//
//     HeavyHiggsBranch->SmallerNeutrinoDeltaR = Quartet.GetSmallerTripletDeltaR();
//     HeavyHiggsBranch->SmallerNeutrinoDeltaRap = Quartet.GetSmallerTripletDeltaRap();
//     HeavyHiggsBranch->SmallerNeutrinoDeltaPhi = Quartet.GetSmallerTripletDeltaPhi();

    ChargedHiggsBranch.TopBdt = Triplet.Bdt();
    ChargedHiggsBranch.HeavyHiggsTag = Triplet.Tag();
return ChargedHiggsBranch;
}

void hanalysis::HChargedHiggsLeptonicTagger::DefineVariables()
{

    Print(HNotification , "Define Variables");

    AddVariable(Branch.HeavyHiggsPt, "HeavyHiggsPt");

    AddVariable(Branch.TopDeltaR, "TopDeltaR");
    AddVariable(Branch.TopDeltaRap, "TopDeltaRap");
    AddVariable(Branch.TopDeltaPhi, "TopDeltaPhi");

//     AddObservable(Branch.LargerWDeltaR, "LargerWDeltaR");
//     AddObservable(Branch.LargerWDeltaRap, "LargerWDeltaRap");
//     AddObservable(Branch.LargerWDeltaPhi, "LargerWDeltaPhi");
//
//     AddObservable(Branch.SmallerWDeltaR, "SmallerWDeltaR");
//     AddObservable(Branch.SmallerWDeltaRap, "SmallerWDeltaRap");
//     AddObservable(Branch.SmallerWDeltaPhi, "SmallerWDeltaPhi");
//
//     AddObservable(Branch.LargerNeutrinoDeltaR, "LargerNeutrinoDeltaR");
//     AddObservable(Branch.LargerNeutrinoDeltaRap, "LargerNeutrinoDeltaRap");
//     AddObservable(Branch.LargerNeutrinoDeltaPhi, "LargerNeutrinoDeltaPhi");
//
//     AddObservable(Branch.SmallerNeutrinoDeltaR, "SmallerNeutrinoDeltaR");
//     AddObservable(Branch.SmallerNeutrinoDeltaRap, "SmallerNeutrinoDeltaRap");
//     AddObservable(Branch.SmallerNeutrinoDeltaPhi, "SmallerNeutrinoDeltaPhi");

    AddVariable(Branch.TopBdt, "TopBdt");

    AddSpectator(Branch.HeavyHiggsTag, "HeavyHiggsTag");
    AddSpectator(Branch.HeavyHiggsMass, "HeavyHiggsMass");

    Print(HNotification, "Variables defined");

}


std::vector< HChargedHiggsLeptonicBranch> hanalysis::HChargedHiggsLeptonicTagger::GetBranches(HEvent &Event, const HObject::Tag Tag)
{
    Print(HInformation, "Get Higgs Tags");

    JetTag.HeavyParticles = {TopId, ChargedHiggsId};
    HJets Jets = Event.GetJets()->GetStructuredTaggedJets(JetTag);

    //     Jets = bottom_tagger_.GetJetBdt(Jets, BottomReader); // TODO reenable this

    HJets Leptons = Event.GetLeptons()->GetTaggedJets(JetTag);
    Print(HInformation, "Numeber of Jets", Jets.size(), Leptons.size());

    std::vector<HDoublet> Doublets = TopLeptonicTagger.GetBdt(Jets, Leptons, TopLeptonicReader);

//     fastjet::PseudoJet MissingEt = Event.GetJets()->GetMissingEt();
//     HJets Neutrinos = Event.GetParticles()->GetNeutrinos();

    Print(HInformation, "Number of Doublets", Doublets.size());

    std::vector<HTriplet> Triplets;
    for (const auto & Doublet : Doublets) {
        for (const auto & Jet : Jets) {
            if (Doublet.Singlet1() == Jet) continue;
            HTriplet Triplet(Doublet, Jet);
            Triplet.SetTag(GetTag(Triplet));
            if (Triplet.Tag() != Tag) continue;
//             std::vector<HQuartet31> PreQuartets;
//             PreQuartets = GetQuartet(Quartet, MissingEt, Neutrinos, Tag);
//             for (auto & Quartet : PreQuartets) {
                Triplets.emplace_back(Triplet);
//             }
        }
    }
    Print(HInformation, "Numeber of Triplets", Triplets.size());


    if (Tag == kSignal && Triplets.size() > 1) {
        Print(HError, "Higgs Candidates", Triplets.size());
        std::sort(Triplets.begin(), Triplets.end());
        Triplets.erase(Triplets.begin() + 1, Triplets.end());
    }

    std::vector<HChargedHiggsLeptonicBranch> ChargedHiggsBranches;
    for (const auto & Triplet : Triplets) ChargedHiggsBranches.emplace_back(GetBranch(Triplet));

    return ChargedHiggsBranches;

}

hanalysis::HObject::Tag hanalysis::HChargedHiggsLeptonicTagger::GetTag(const HTriplet &Triplet)
{
    Print(HInformation, "Get Triple Tag");

    if (Triplet.Doublet().Tag() == kBackground) return kBackground;
//     if (Quartet.GetDoublet2().Tag() == HBackground) return HBackground;
    // TODO check the following
    if (Triplet.Doublet().Singlet1().user_index() != -Triplet.Singlet().user_index()) return kBackground;
    return kSignal;
}

std::vector<hanalysis::HTriplet>  hanalysis::HChargedHiggsLeptonicTagger::GetBdt(const std::vector<HDoublet> &Doublets,const std::vector<fastjet::PseudoJet> Jets, const Reader &Reader)
{
    Print(HInformation, "Get Bdt");

    std::vector<HTriplet> Triplets;
    for (const auto & Doublet : Doublets) {
        for (const auto & Jet : Jets) {
            if (Doublet.Singlet1() == Jet) continue;
            HTriplet Triplet(Doublet, Jet);
            Triplet.SetTag(GetTag(Triplet));
//             std::vector<HQuartet31> PreQuartets;
//             PreQuartets = GetQuartets(Quartet, MissingEt);
//             for (auto & Quartet : PreQuartets) {
                Branch = GetBranch(Triplet);
                Triplet.SetBdt(Reader.Bdt());
                Triplets.emplace_back(Triplet);
//             }
        }
    }
    return Triplets;
}
