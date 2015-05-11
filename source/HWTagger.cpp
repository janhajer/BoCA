# include "HWTagger.hh"

hanalysis::HWTagger::HWTagger()
{
    //     DebugLevel = hanalysis::HObject::kDebug;
    Print(kNotification, "Constructor");
    DefineVariables();

}

void hanalysis::HWTagger::SetTagger(const HBottomTagger &NewBottomTagger)
{
    Print(kNotification, "Set Tagger");
    BottomTagger = NewBottomTagger;
    WMassWindow = 20;
    DefineVariables();
}

void hanalysis::HWTagger::DefineVariables()
{
    Print(kNotification , "Define Variables");
    SetTaggerName("WHadronic");
    ClearVectors();


    AddObservable(Branch.Mass, "Mass");
    AddObservable(Branch.Rap, "Rap");
    AddObservable(Branch.Phi, "Phi");
    AddObservable(Branch.Pt, "Pt");
    AddObservable(Branch.Ht, "Ht");

    AddObservable(Branch.DeltaPt, "DeltaPt");
    AddObservable(Branch.DeltaPhi, "DeltaPhi");
    AddObservable(Branch.DeltaRap, "DeltaRap");
    AddObservable(Branch.DeltaR, "DeltaR");

    AddObservable(Branch.Bdt, "Bdt");

    AddSpectator(Branch.Tag, "Tag");
}

HWBranch hanalysis::HWTagger::GetBranch(const Doublet &doublet) const
{
    Print(kInformation, "FillPairTagger", doublet.Bdt());
    HWBranch WBranch;
    WBranch.Mass = doublet.Jet().m();
    WBranch.Rap = doublet.Jet().rap();
    WBranch.Phi = doublet.Jet().phi();
    WBranch.Pt = doublet.Jet().pt();
    WBranch.Ht = doublet.Ht();
    WBranch.DeltaPt = std::abs(doublet.DeltaPt());
    WBranch.DeltaR = doublet.DeltaR();
    WBranch.DeltaRap = std::abs(doublet.DeltaRap());
    WBranch.DeltaPhi = std::abs(doublet.DeltaPhi());
    WBranch.Bdt = doublet.Bdt();
    WBranch.Tag = doublet.Tag();
    return WBranch;

}

std::vector<HWBranch> hanalysis::HWTagger::GetBranches(hanalysis::HEvent &Event, const hanalysis::HObject::Tag Tag)
{
    Print(kInformation, "Get W Tags");
    std::vector<Doublet> doublets;

    Jets jets = GetJets(Event);
    Print(kInformation, "Jets Number", jets.size());
    jets = BottomTagger.GetJetBdt(jets, BottomReader);
    Print(kInformation, "Bottom Tagger Number", jets.size());

    Jets WKids = GetWDaughters(Event);
    int WHadId = WId;
    if (WKids.size() > 0) WHadId = GetWHadId(WKids);

    // 2 Jets form 1 W
    Jets JetPairs;
    if (WKids.size() > 0) std::sort(jets.begin(), jets.end(), MinDeltaR(WKids.at(0)));
    if (Tag == kSignal && jets.size() > 1) JetPairs.push_back(jets.front());
    if (Tag == kBackground && jets.size() > 1) JetPairs.insert(JetPairs.end(), jets.begin() + 1 , jets.end());

    if (WKids.size() > 1) std::sort(jets.begin(), jets.end(), MinDeltaR(WKids.at(1)));
    if (Tag == kSignal && jets.size() > 1) JetPairs.push_back(jets.front());
    if (Tag == kSignal && JetPairs.size() != 2) Print(kError, "lost the W jets", JetPairs.size());

    for (const auto & Jet1 : JetPairs) {
        for (const auto & Jet2 : JetPairs) {
            if (Jet1.delta_R(Jet2) < DetectorGeometry.JetConeSize) continue;
            Doublet doublet(Jet1, Jet2);
            if (Tag == kSignal && std::abs(doublet.Jet().m() - WMass) > WMassWindow) continue;
            doublets.push_back(doublet);
        }
    }


    Jets WParticles = Event.GetParticles()->Generator();
    WParticles = RemoveIfWrongParticle(WParticles, WHadId);
    fastjet::PseudoJet WParticle;
    if (Tag == kSignal) {
        if (WParticles.size() == 1) {
            WParticle = WParticles.front();
            std::sort(jets.begin(), jets.end(), MinDeltaR(WParticle));
        } else Print(kError, "Where is the W?", WParticles.size());
    }

//  1 Jet (2 subjets) form(s) 1 W
    Jets WJets;
    if (Tag == kSignal && jets.size() > 1) WJets.push_back(jets.front());
    if (Tag == kBackground && jets.size() > 1) WJets.insert(WJets.end(), jets.begin() + 1 , jets.end());
    for (const auto & Jet : WJets) {
        Jets Pieces = GetSubJets(Jet, 2);
        Pieces = BottomTagger.GetJetBdt(Pieces, BottomReader);
        for (const auto & Piece1 : Pieces) {
            for (const auto & Piece2 : Pieces) {
                if (Piece1 == Piece2) continue;
                Doublet doublet(Piece1, Piece2);
                if (Tag == kSignal && std::abs(doublet.Jet().m() - WMass) > WMassWindow) continue;
                if (Tag == kSignal && doublet.Jet().delta_R(WParticle) > DetectorGeometry.JetConeSize) continue;
                if (Tag == kBackground && doublet.Jet().delta_R(WParticle) < DetectorGeometry.JetConeSize) continue;
                doublets.push_back(doublet);
            }
        }
    }
//
//     Jets TopParticles = Event.GetParticles()->Generator();
//     TopParticles.erase(std::remove_if(TopParticles.begin(), TopParticles.end(), WrongId(sgn(WHadId)*TopId)), TopParticles.end());
//     Print(kInformation, "Particle size", TopParticles.size());
//     if (TopParticles.size() != 1) Print(kError, "Where is the Top?", TopParticles.size());
//     std::sort(jets.begin(), jets.end(), MinDeltaR(TopParticles.front()));
//     if (Tag == HSignal && jets.size() > 1) jets.erase(jets.begin() + 1, jets.end());
//     if (Tag == HBackground && jets.size() > 0) jets.erase(jets.begin());

    // W is in 2 of 3 subjets
    for (const auto & Jet : WJets) {
        Jets Pieces = GetSubJets(Jet, 3);
        Pieces = BottomTagger.GetJetBdt(Pieces, BottomReader);
        for (const auto & Piece1 : Pieces)
            for (const auto & Piece2 : Pieces) {
                if (Piece1 == Piece2) continue;
                Doublet doublet(Piece1, Piece2);
                if (Tag == kSignal && std::abs(doublet.Jet().m() - WMass) > WMassWindow) continue;
                if (Tag == kSignal && doublet.Jet().delta_R(WParticle) > DetectorGeometry.JetConeSize) continue;
                if (Tag == kBackground && doublet.Jet().delta_R(WParticle) < DetectorGeometry.JetConeSize) continue;
                doublets.push_back(doublet);
            }
    }

    // W is in 1 of 2 subjet
    for (const auto & Jet : jets)  {
        Jets Pieces = GetSubJets(Jet, 2);
        Pieces = BottomTagger.GetJetBdt(Pieces, BottomReader);
        for (const auto & Piece : Pieces) {
            Doublet doublet(Piece);
            if (Tag == kSignal && std::abs(doublet.Jet().m() - WMass) > WMassWindow) continue;
            if (Tag == kSignal && doublet.Jet().delta_R(WParticle) > DetectorGeometry.JetConeSize) continue;
            if (Tag == kBackground && doublet.Jet().delta_R(WParticle) < DetectorGeometry.JetConeSize) continue;
            doublets.push_back(doublet);
        }
    }



    const int WNumber = 1; // MUST BE 1 FOR THE ANALYSIS
//     std::sort(doublets.begin(), doublets.end(), MinDeltaR(WParticles));
    std::sort(doublets.begin(), doublets.end(), SortByMass(WMass));
    if (Tag == kSignal && doublets.size() > WNumber) doublets.erase(doublets.begin() + WNumber, doublets.end());
    if (Tag == kBackground && doublets.size() > WNumber) doublets.erase(doublets.begin());

    std::vector<HWBranch> Branches;
    for (const auto & doublet : doublets) Branches.push_back(GetBranch(doublet));
    return Branches;
}

// hanalysis::HObject::HTag hanalysis::HWTagger::GetTag(const Doublet &doublet)
// {
//     Print(kInformation, "Get doublet Tag");
//
//     if (std::abs(doublet.Singlet1().user_info<JetInfo>().MaximalId()) != WId) return HBackground;
//
//     if (doublet.Singlet1().user_info<JetInfo>().MaximalId() != doublet.Singlet2().user_info<JetInfo>().MaximalId()) return HBackground;
//
//     return HSignal;
// }

// hanalysis::HObject::HTag hanalysis::HWTagger::GetTag(const fastjet::PseudoJet &Singlet)
// {
//     Print(kInformation, "Get Singlet Tag");
//
//     if (std::abs(Singlet.user_info<JetInfo>().MaximalId()) != WId) return HBackground;
//     return HSignal;
// }

Jets hanalysis::HWTagger::GetWDaughters(HEvent &Event)
{
    Jets WKids = Event.GetParticles()->Generator();
    WKids = RemoveIfWrongAbsMother(WKids, WId);
    if (WKids.size() != 4) Print(kError, "Where is the W 1?", WKids.size());

    WKids = RemoveIfLetpons(WKids);
    if (WKids.size() != 2) Print(kError, "Where is the W 2?", WKids.size());
    else Print(kInformation, "W Daughters", GetParticleName(WKids.at(0).user_info<hanalysis::JetInfo>().constituents().front().Family().ParticleId), GetParticleName(WKids.at(1).user_info<hanalysis::JetInfo>().constituents().front().Family().ParticleId), GetParticleName(WKids.at(0).user_info<hanalysis::JetInfo>().constituents().front().Family().Mother1Id), GetParticleName(WKids.at(1).user_info<hanalysis::JetInfo>().constituents().front().Family().Mother1Id));
    return WKids;
}

int hanalysis::HWTagger::GetWHadId(const Jets &jets)
{
    if (jets.size() < 1) return WId;
    else return jets.at(0).user_info<hanalysis::JetInfo>().constituents().front().Family().Mother1Id;
}


std::vector<hanalysis::Doublet> hanalysis::HWTagger::GetBdt(const Jets &jets, const hanalysis::HReader &WReader, hanalysis::HBottomTagger &BottomTagger, hanalysis::HReader &BottomReader)
{
    Print(kInformation, "Get doublet Bdt");

    // W in 2 jets
    std::vector<Doublet>  doublets;
    for (auto Jet1 = jets.begin(); Jet1 != jets.end(); ++Jet1)
        for (auto Jet2 = Jet1 + 1; Jet2 != jets.end(); ++Jet2) {
            Doublet doublet(*Jet1, *Jet2);
            if (doublet.DeltaR() < DetectorGeometry.MinCellResolution) continue;
            if (std::abs(doublet.Jet().m() - WMass) > WMassWindow) continue;
            Branch = GetBranch(doublet);
            doublet.SetBdt(WReader.Bdt());
            doublets.push_back(doublet);
        }

//  1 Jet (2 subjets) form(s) 1 W
    for (const auto & Jet : jets)  {
        Jets Pieces = GetSubJets(Jet, 2);
        Pieces = BottomTagger.GetJetBdt(Pieces, BottomReader);
        for (const auto & Piece1 : Pieces) {
            for (const auto & Piece2 : Pieces) {
                if (Piece1 == Piece2) continue;
                Doublet doublet(Piece1, Piece2);
                if (doublet.DeltaR() < DetectorGeometry.MinCellResolution) continue;
                if (std::abs(doublet.Jet().m() - WMass) > WMassWindow) continue;
                Branch = GetBranch(doublet);
                doublet.SetBdt(WReader.Bdt());
                doublets.push_back(doublet);
            }
        }
    }


// W is in 2 of 3 subjets
    for (const auto & Jet : jets)  {
        Jets Pieces = GetSubJets(Jet, 3);
        Pieces = BottomTagger.GetJetBdt(Pieces, BottomReader);
        for (const auto & Piece1 : Pieces)
            for (const auto & Piece2 : Pieces) {
                if (Piece1 == Piece2)continue;
                Doublet doublet(Piece1, Piece2);
                if (doublet.DeltaR() < DetectorGeometry.MinCellResolution) continue;
                if (std::abs(doublet.Jet().m() - WMass) > WMassWindow) continue;
                Branch = GetBranch(doublet);
                doublet.SetBdt(WReader.Bdt());
                doublets.push_back(doublet);
            }
    }

    //  1 of 2 subjets are the W
    for (const auto & Jet : jets)  {
      Jets Pieces = GetSubJets(Jet, 2);
      Pieces = BottomTagger.GetJetBdt(Pieces, BottomReader);
      for (const auto & Piece : Pieces) {
          Doublet doublet(Piece);
          if (std::abs(doublet.Jet().m() - WMass) > WMassWindow) continue;
          Branch = GetBranch(doublet);
          doublet.SetBdt(WReader.Bdt());
          doublets.push_back(doublet);
        }
    }

    std::sort(doublets.begin(), doublets.end());
    doublets.erase(doublets.begin() + std::min(max_combi(), int(doublets.size())), doublets.end());
    return doublets;
}

std::vector<hanalysis::Doublet> hanalysis::HWTagger::GetBdt(const Jets &jets, const hanalysis::HReader &WReader)
{
  Print(kInformation, "Get doublet Bdt");

  std::vector<Doublet>  doublets;
  for (auto Jet1 = jets.begin(); Jet1 != jets.end(); ++Jet1)
    for (auto Jet2 = Jet1 + 1; Jet2 != jets.end(); ++Jet2) {
      Doublet doublet(*Jet1, *Jet2);
//       if (doublet.DeltaR() < DetectorGeometry.MinCellResolution) continue;
//       if (std::abs(doublet.Jet().m() - WMass) > WMassWindow) continue;
      Branch = GetBranch(doublet);
      doublet.SetBdt(WReader.Bdt());
      doublets.push_back(doublet);
    }

  for (const auto & Jet : jets) {
      Doublet doublet(Jet);
//       if (std::abs(doublet.Jet().m() - WMass) > WMassWindow) continue;
      Branch = GetBranch(doublet);
      doublet.SetBdt(WReader.Bdt());
      doublets.push_back(doublet);
    }

    std::sort(doublets.begin(), doublets.end());
    doublets.erase(doublets.begin() + std::min(max_combi(), int(doublets.size())), doublets.end());
    return doublets;
}

std::vector<hanalysis::Doublet> hanalysis::HWTagger::GetPairBdt(const Jets &jets, const hanalysis::HReader &WReader)
{
  Print(kInformation, "Get doublet Bdt");

  std::vector<Doublet>  doublets;
  for (auto Jet1 = jets.begin(); Jet1 != jets.end(); ++Jet1)
    for (auto Jet2 = Jet1 + 1; Jet2 != jets.end(); ++Jet2) {
      Doublet doublet(*Jet1, *Jet2);
//       if (doublet.DeltaR() < DetectorGeometry.MinCellResolution) continue;
//       if (std::abs(doublet.Jet().m() - WMass) > WMassWindow) continue;
      Branch = GetBranch(doublet);
      doublet.SetBdt(WReader.Bdt());
      doublets.push_back(doublet);
    }

    std::sort(doublets.begin(), doublets.end());
    doublets.erase(doublets.begin() + std::min(max_combi(), int(doublets.size())), doublets.end());
    return doublets;
}

std::vector<hanalysis::Doublet> hanalysis::HWTagger::GetSingletBdt(const Jets &jets, const hanalysis::HReader &WReader)
{
  Print(kInformation, "Get doublet Bdt");

  std::vector<Doublet>  doublets;
    for (const auto & Jet : jets) {
      Doublet doublet(Jet);
//       if (std::abs(doublet.Jet().m() - WMass) > WMassWindow) continue;
      Branch = GetBranch(doublet);
      doublet.SetBdt(WReader.Bdt());
      doublets.push_back(doublet);
    }

    std::sort(doublets.begin(), doublets.end());
    doublets.erase(doublets.begin() + std::min(max_combi(), int(doublets.size())), doublets.end());
    return doublets;
}


hanalysis::Doublet hanalysis::HWTagger::GetBdt(Doublet &doublet, const hanalysis::HReader &WReader)
{
    Print(kInformation, "Get doublet Bdt");

    Branch = GetBranch(doublet);
    doublet.SetBdt(WReader.Bdt());
    return doublet;
}

// std::vector<ParticleBranch> hanalysis::HWTagger::GetconstituentBranches()
// {
//
//     Print(kInformation, "Get Higgs Tags");
//
//     std::vector<Doublet> JetPairs;
//
//     Print(kInformation, "Number of Jet Pairs", JetPairs.size());
//
//     std::vector<ParticleBranch> constituentBranches;
//     for (const auto & JetPair : JetPairs) for (const auto & constituent : JetPair.constituents()) constituentBranches.push_back(GetBranch(constituent));
//     return constituentBranches;
// }
//
// ParticleBranch hanalysis::HWTagger::GetBranch(const HKinematics &Vector)
// {
//     Print(kInformation, "Fill constituent Branch");
//
//     ParticleBranch constituentBranch;
//     constituentBranch.Pt = Vector.GetPt();
//     constituentBranch.Rap = Vector.GetRap();
//     constituentBranch.Phi = Vector.GetPhi();
//     return constituentBranch;
// }
