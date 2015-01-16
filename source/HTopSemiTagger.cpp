# include "HTopSemiTagger.hh"

hanalysis::HTopSemiTagger::HTopSemiTagger(HBottomTagger *const NewBottomTagger, HWSemiTagger *const NewWSemiTagger)
{
//     DebugLevel = hanalysis::HObject::HDebug;

    Print(HNotification, "Constructor");
    BottomTagger = NewBottomTagger;
    BottomReader = new HReader(BottomTagger);
    WSemiTagger = NewWSemiTagger;
    WSemiReader = new HReader(WSemiTagger);
    SetTaggerName("TopSemi");
    Branch = new HTopSemiBranch();
    JetTag = new HJetTag();
    DefineVariables();
    TopWindow = 50;
// //     std::string TopCut = "TopSemi.Mass>" + std::to_string(TopMass - TopWindow) + "&&TopSemi.Mass<" + std::to_string(TopMass + TopWindow);
//     Cut = TopCut.c_str();
}

hanalysis::HTopSemiTagger::~HTopSemiTagger()
{
    Print(HNotification, "Destructor");
    delete Branch;
    delete JetTag;
    delete BottomReader;
    delete WSemiReader;
}

void hanalysis::HTopSemiTagger::DefineVariables()
{

    Print(HNotification , "Define Variables");

    Observables.push_back(NewObservable(&Branch->Mass, "Mass"));
    Observables.push_back(NewObservable(&Branch->Pt, "Pt"));
    Observables.push_back(NewObservable(&Branch->Rap, "Rap"));
    Observables.push_back(NewObservable(&Branch->Phi, "Phi"));

    Observables.push_back(NewObservable(&Branch->BottomPt, "BottomPt"));
    Observables.push_back(NewObservable(&Branch->WPt, "WPt"));

    Observables.push_back(NewObservable(&Branch->DeltaPhi, "DeltaPhi"));
    Observables.push_back(NewObservable(&Branch->DeltaRap, "DeltaRap"));
    Observables.push_back(NewObservable(&Branch->DeltaR, "DeltaR"));

    Observables.push_back(NewObservable(&Branch->Bdt, "Bdt"));
    Spectators.push_back(NewObservable(&Branch->Tag, "Tag"));

    Print(HNotification, "Variables defined");

}

void hanalysis::HTopSemiTagger::FillBranch(HTopSemiBranch *const TopSemiBranch, const HTriplet &Triplet)
{
    Print(HInformation, "Fill Top Tagger", Triplet.GetBdt());

    TopSemiBranch->Mass = Triplet.GetTripletJet().m();
    TopSemiBranch->Pt = Triplet.GetTripletJet().pt();
    TopSemiBranch->Rap = Triplet.GetTripletJet().rap();
    TopSemiBranch->Phi = Triplet.GetTripletJet().phi();

    TopSemiBranch->BottomPt = Triplet.GetSinglet().pt();
    TopSemiBranch->WPt = Triplet.GetDoubletJet().pt();

    TopSemiBranch->DeltaR = Triplet.GetDeltaR();
    TopSemiBranch->DeltaRap = Triplet.GetDeltaRap();
    TopSemiBranch->DeltaPhi = Triplet.GetDeltaPhi();

    TopSemiBranch->Bdt = Triplet.GetBdt();
    TopSemiBranch->Tag = Triplet.GetTag();

}

void hanalysis::HTopSemiTagger::FillBranch(const HTriplet &Triple)
{
    Print(HInformation, "Fill Top Tagger", Triple.GetBdt());
    FillBranch(Branch, Triple);
}

struct SortByTopMass {
    inline bool operator()(const hanalysis::HTriplet &Triplet1, const hanalysis::HTriplet &Triplet2) {
        return (std::abs(Triplet1.GetTripletJet().m() - Triplet1.TopMass) < std::abs(Triplet2.GetTripletJet().m() - Triplet2.TopMass));
    }
};


std::vector<HTopSemiBranch *> hanalysis::HTopSemiTagger::GetBranches(HEvent *const Event, const HObject::HTag Tag)
{

    Print(HInformation, "Get Top Tags");

    JetTag->HeavyParticles = {TopId};
    HJets Jets = Event->GetJets()->GetStructuredTaggedJets(JetTag);
    Print(HInformation, "Jet Number", Jets.size());

    Jets = BottomTagger->GetBdt(Jets, BottomReader);

    HJets Leptons = Event->GetLeptons()->GetTaggedJets(JetTag);
    Print(HInformation, "Lepton Number", Leptons.size());

    fastjet::PseudoJet MissingEt = Event->GetJets()->GetMissingEt();
    std::vector<HDoublet> Doublets = WSemiTagger->GetBdt(Leptons, MissingEt, WSemiReader);
    Print(HInformation, "Number Doublets", Doublets.size());

    std::vector<HTriplet> Triplets;
    for (const auto & Jet : Jets) {
        std::vector<HTriplet> PreTriplets;
        for (const auto & Doublet : Doublets) {
            HTriplet Triplet(Doublet, Jet);
            Triplet.SetTag(GetTag(Triplet));
            if (Triplet.GetTag() != Tag) continue;
            if (Tag == HSignal && std::abs(Triplet.GetTripletJet().m() - TopMass) > TopWindow) continue;
            PreTriplets.push_back(Triplet);
        }
        if (PreTriplets.size() > 1) std::sort(PreTriplets.begin(), PreTriplets.end(), SortByTopMass());
        if (PreTriplets.size() > 0) Triplets.push_back(PreTriplets.front());
    }

//     if (Tag == HBackground || (Tag == HSignal && Triplets.size() < 1)) {
//         for (const auto Jet : Jets) {
//             fastjet::PseudoJet EmptyJet(0, 0, 0, 0);
//             HDoublet Doublet(EmptyJet, EmptyJet);
//             Doublet.SetBdt(Jet.user_info<HJetInfo>().GetBdt());
//             HTriplet Triplet(Doublet, Jet);
//             Triplet.SetTag(GetTag(Jet));
//             if (Triplet.GetTag() != Tag) continue;
//             if (Tag == HSignal && std::abs(Triplet.GetTripletJet().m() - TopMass) > TopWindow) continue;
//             Triplets.push_back(Triplet);
//         }
//     }

    if (Tag == HSignal && Triplets.size() > 1) {
        std::sort(Triplets.begin(), Triplets.end(), SortByTopMass());
        Triplets.erase(Triplets.begin() + 1, Triplets.end());
    }

    Print(HInformation, "Number Triplets", Triplets.size());

    std::vector<HTopSemiBranch *> TopSemiBranches;
    for (const auto & Triplet : Triplets) {
        HTopSemiBranch *TopSemiBranch = new HTopSemiBranch();
        FillBranch(TopSemiBranch, Triplet);
        TopSemiBranches.push_back(TopSemiBranch);
    }
    return TopSemiBranches;
}

hanalysis::HObject::HTag hanalysis::HTopSemiTagger::GetTag(const hanalysis::HTriplet &Triplet) const
{
    Print(HInformation, "Get Triple Tag", GetParticleName(Triplet.GetSinglet().user_index()), GetParticleName(Triplet.GetDoublet().GetJet1().user_index()));

    HJetInfo BJetInfo = Triplet.GetSinglet().user_info<HJetInfo>();
    BJetInfo.ExtractFraction(BottomId);
    BJetInfo.PrintAllInfos(HInformation);
    HJetInfo W1JetInfo = Triplet.GetDoublet().GetJet1().user_info<HJetInfo>();
    W1JetInfo.ExtractFraction(WId);
    W1JetInfo.PrintAllInfos(HInformation);

    if (std::abs(W1JetInfo.GetMaximalId()) != WId) return HBackground;
    if (std::abs(BJetInfo.GetMaximalId()) != BottomId) return HBackground;
    if (sgn(BJetInfo.GetMaximalId()) != sgn(W1JetInfo.GetMaximalId())) return HBackground;
    return HSignal;
}

hanalysis::HObject::HTag hanalysis::HTopSemiTagger::GetTag(const fastjet::PseudoJet &Singlet) const
{
    Print(HInformation, "Get Triple Tag", GetParticleName(Singlet.user_index()));

    HJetInfo JetInfo = Singlet.user_info<HJetInfo>();
    JetInfo.ExtractFraction(TopId);
    JetInfo.PrintAllInfos(HInformation);

    if (std::abs(JetInfo.GetMaximalId()) != TopId) return HBackground;
    return HSignal;
}

std::vector<hanalysis::HTriplet>  hanalysis::HTopSemiTagger::GetBdt(const std::vector<HDoublet> &Doublets, const HJets &Jets,const fastjet::PseudoJet &MissingEt, const HReader *const Reader)
{

    Print(HInformation, "Get Bdt");

    std::vector<HTriplet> Triplets;
    for (const auto & Jet : Jets) {
        std::vector<HTriplet> PreTriplets;
        for (const auto & Doublet : Doublets) {
            HTriplet Triplet(Doublet, Jet);
            FillBranch(Triplet);
            Triplet.SetBdt(Reader->GetBdt());
            PreTriplets.push_back(Triplet);
        }
        if (PreTriplets.size() > 1) std::sort(PreTriplets.begin(), PreTriplets.end(), SortByTopMass());
        if (PreTriplets.size() > 0) Triplets.push_back(PreTriplets.front());
    }

//     if (Triplets.size() < 1) {
//     for (const auto Jet : Jets) {
//         fastjet::PseudoJet EmptyJet(0, 0, 0, 0);
//         if (Jet.has_user_info<HJetInfo>()) {
//             HJetInfo *JetInfo = new HJetInfo();
//             JetInfo->SetJetFamily(Jet.user_info<HJetInfo>().GetJetFamily());
//             JetInfo->SetBdt(Jet.user_info<HJetInfo>().GetBdt());
//             EmptyJet.set_user_info(JetInfo);
//         }
//         HDoublet Doublet(EmptyJet, EmptyJet);
//         Doublet.SetBdt(Jet.user_info<HJetInfo>().GetBdt());
//         HTriplet Triplet(Doublet, Jet);
//         FillBranch(Triplet);
//         Triplet.SetBdt(Reader->GetBdt());
//         Triplets.push_back(Triplet);
//         }
//     }

    std::sort(Triplets.begin(), Triplets.end());
    Triplets.erase(Triplets.begin() + std::min(MaxCombi, int(Triplets.size())), Triplets.end());

    return Triplets;
}
