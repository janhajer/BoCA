# include "HAnalysisDiscriminator.hh"

Discriminator::HAnalysis::HAnalysis()
{

    Print(1, "Constructor");

    JetTag = new Discriminator::HJetTag();

    SubStructure = new Analysis::HSubStructure();

//     DebugLevel = 3;

}

Discriminator::HAnalysis::~HAnalysis()
{

    Print(1, "Destructor");

    delete JetTag;

    delete SubStructure;

}

vector<string> Discriminator::HAnalysis::GetStudyNames() const
{

//     return  {"Higgs", "Top", "Jet", "Test"};
    return {"Higgs", "Top"};

}

vector<Analysis::HFile *> Discriminator::HAnalysis::GetFiles(const string StudyName) const
{
    Print(1, "Set File Vector", StudyName);

    vector<Analysis::HFile *> Files;

    if (StudyName != "Higgs") {

        Analysis::HDelphes::HFile *Background = new Analysis::HDelphes::HFile("pp-bbtt-bblvlv", "background");
        Background->Crosssection = 3.215; // pb
        Background->Error = 0.012; // pb
        Files.push_back(Background);

//         HFile *Background2 = new HFile("pp-bbtt-bblvlv-HIGGS", "background");
//         Background2->Crosssection = 1.243; // pb
//         Background2->Error = 0.1309; // pb
//         Files.push_back(Background2);

    }

//     HFile *Even = new HFile("pp-x0tt-bblvlv", "even");
//     Even->Crosssection = 0.02079; // pb
//     Even->Error = 0.000078; // pb
// //     Even->TagString="tag_2";
//     Files.push_back(Even);

//     HFile *Mix = new HFile("pp-x0tt-bblvlv", "mix");
//     Mix->Crosssection = 0.01172; // pb
//     Mix->Error = 0.000045; // pb
// //     Mix->TagString="tag_2";
//     Files.push_back(Mix);
//
//     HFile *Odd = new HFile("pp-x0tt-bblvlv", "odd");
//     Odd->Crosssection = 0.008951; // pb
//     Odd->Error = 0.000035; // pb
// //     Odd->TagString="tag_2";
//     Files.push_back(Odd);

    Analysis::HDelphes::HFile *Signal = new Analysis::HDelphes::HFile("pp-htt-bblvlv", "signal");
    Signal->Crosssection = 0.01419; // pb
    Signal->Error = 0.000067; // pb
//     Odd->TagString="tag_2";
    Files.push_back(Signal);

    Print(1, "Files prepared");

    return Files;

}


void Discriminator::HAnalysis::NewBranches(ExRootTreeWriter *TreeWriter)
{
    Print(1, "New File");

    CandidateBranch = TreeWriter->NewBranch("Candidate", HCandidateBranch::Class());
    LeptonBranch = TreeWriter->NewBranch("Lepton", HLeptonBranch::Class());
    ConstituentBranch = TreeWriter->NewBranch("Constituent", HParticleBranch::Class());

}

int Discriminator::HJetTag::GetBranchId(const int ParticleId, int BranchId)
{

//     if (HeavyParticles.find(abs(BranchId)) != end(HeavyParticles)) DebugLevel =4;

    Print(3, "Get Branch Id", ParticleId, BranchId);

    if (
        RadiationParticles.find(abs(ParticleId)) != end(RadiationParticles) &&
        HeavyParticles.find(abs(BranchId)) == end(HeavyParticles)
    ) {
        BranchId = IsrId;
    } else if (
        HeavyParticles.find(abs(ParticleId)) != end(HeavyParticles) &&
        HeavyParticles.find(abs(BranchId)) == end(HeavyParticles)
    ) {
        BranchId = ParticleId;
    }

    Print(3, "Branch Id", BranchId);
//     DebugLevel =1;
    return BranchId;

}

bool Discriminator::HAnalysis::Analysis(Analysis::HEvent *const Event, const string StudyName)
{

    Print(2, "Analysis", StudyName);

    const vector<PseudoJet> Leptons = GetLeptonJets(Event);

    if (Leptons.size() < 2) {

        Print(1, "Not enough Leptons", Leptons.size());
        return 0;

    }

    ++LeptonEventCounter;

    // Higgs stuff

    const vector<PseudoJet> CandidateJets = Event->GetHiggsTopCandidates(JetTag);

    if (CandidateJets.size() < 1) {

        Print(0, "No Candidates", CandidateJets.size());

        return 0;

    } else Print(2, "Number of Candidates", CandidateJets.size());

    int CandidateCounter = 0;
    bool HasCandidate = 0;
//     bool HiggsCounter = 0;

    for (const auto & CandidateJet : CandidateJets) {

        Print(3, "Candidate", CandidateJet.user_index());

        if (StudyName == "Higgs") {

            if (CandidateCounter > 0) {

                Print(2, "Number of Higgs", CandidateCounter);
                break;

            }

            if (CandidateJet.user_index() != CpvHiggsId && CandidateJet.user_index() != HiggsId) {

                Print(3, "Not a Higgs");
                continue;

            }

            ++CandidateCounter;

            Print(3, "Higgs", CandidateCounter);

        }

        if (StudyName == "Top") {

            if (CandidateCounter > 0) {

                Print(2, "Number of Higgs", CandidateCounter);
                break;

            }

            if (abs(CandidateJet.user_index()) != TopId) {

                Print(3, "Not a Top");
                continue;

            }

            if (CandidateCounter > 1) Print(2, "Number of Tops", CandidateCounter);

            ++CandidateCounter;

            Print(3, "Top", CandidateCounter);

        }

        if (StudyName == "Jet") {

            if (CandidateJet.user_index() == CpvHiggsId || CandidateJet.user_index() == HiggsId) {

//                 HiggsCounter = 1;
                Print(3, "First Higgs is not a light Jet", CandidateJet.user_index());
                continue;

            }

            if (abs(CandidateJet.user_index()) == TopId) {

                Print(3, "Top is not a light jet", CandidateJet.user_index());
                continue;

            }

            if (CandidateCounter > 2) Print(1, "Number light jets", CandidateCounter);
            ++CandidateCounter;
            Print(2, "Jet", CandidateCounter);

        }

        if (CandidateJet == 0 || CandidateJet.m() <= 0 || CandidateJet.pt() <= 0) {

            Print(0, "Illeagal Candidate", CandidateJet.m());
            continue;

        }

        if (CandidateJet.user_info<Analysis::HJetInfo>().GetMaximalFraction() < .9) break;

        Print(0, "Tag", CandidateJet.user_info<Analysis::HJetInfo>().GetMaximalId(), CandidateJet.user_info<Analysis::HJetInfo>().GetMaximalFraction(), CandidateJet.m());

//         vector<PseudoJet> Constituents = CandidateJet.constituents();
//         sort(Constituents.begin(), Constituents.end(), SortJetByPt());
//         int Counter = 0;
//         for (const auto & Constituent : Constituents) {
//             ++Counter;
//             Print(0, "Const", Constituent.user_index(), Constituent.pt());
//             if (Counter > 9) break;
//         }


        HCandidateBranch *Candidate = static_cast<HCandidateBranch *>(CandidateBranch->NewEntry());

        Candidate->Mass = CandidateJet.m();
        Candidate->Pt = CandidateJet.pt();
        Candidate->Eta = CandidateJet.eta();
        Candidate->Phi = CandidateJet.phi_std();

        Print(2, "Candidate Mass", CandidateJet.m());

        // Tagging

        int UserIndex = abs(CandidateJet.user_index());

        if (UserIndex == CpvHiggsId || UserIndex == HiggsId) {
            Candidate->HiggsTag = 1;
            Candidate->TopTag = 0;

        } else if (UserIndex == TopId) {

            Candidate->TopTag = 1;
            Candidate->HiggsTag = 0;

        } else {

            Candidate->TopTag = 0;
            Candidate->HiggsTag = 0;

        }

        SubStructure->NewEvent();
        if (!SubStructure->GetSubJets(CandidateJet)) {

            Print(0, "No SubJets");

            return 0;

        }

        Candidate->SubJetsDeltaR = SubStructure->GetSubJetsDeltaR();
        Candidate->Asymmetry = SubStructure->GetAsymmetry();
        Candidate->DeltaR = SubStructure->GetDeltaR();
        Candidate->DiPolarity = SubStructure->GetDiPolarity(CandidateJet);

        Candidate->SubJet1Mass = SubStructure->GetSubJet1Mass();
        Candidate->SubJet1Pt = SubStructure->GetSubJet1Pt();
        Candidate->SubJet1DeltaR = SubStructure->GetSubJet1DeltaR();

        Candidate->SubJet2Mass = SubStructure->GetSubJet2Mass();
        Candidate->SubJet2Pt = SubStructure->GetSubJet2Pt();
        Candidate->SubJet2DeltaR = SubStructure->GetSubJet2DeltaR();

        SubStructure->GetIsolation(CandidateJet, Leptons);

        Candidate->IsolationEta = SubStructure->GetIsolationEta();
        Candidate->IsolationPhi = SubStructure->GetIsolationPhi();
        Candidate->IsolationPt = SubStructure->GetIsolationPt();
        Candidate->IsolationDeltaR = SubStructure->GetIsolationDeltaR();
        Candidate->IsolationAngle = SubStructure->GetIsolationAngle();

        Print(3, "Isolation", Candidate->IsolationDeltaR);

        if (!SubStructure->GetConstituents(CandidateJet, ConstituentBranch)) {

            Print(0, "No Constituents");

            return 0;

        }

        Candidate->ConstEta = SubStructure->GetConstituentEta();
        Candidate->ConstPhi = SubStructure->GetConstituentPhi();
        Candidate->ConstDeltaR = SubStructure->GetConstituentDeltaR();
        Candidate->ConstAngle = SubStructure->GetConstituentAngle();

//         CandidateJet.user_info<HJetInfo>().PrintAllInfos(4);
//         Print(1, "Tag", CandidateJet.user_info<HJetInfo>().GetMaximalId(), CandidateJet.user_info<HJetInfo>().GetMaximalFraction(), CandidateJet.m());

        HasCandidate = 1;
    }

    if (HasCandidate) return 1;

    Print(2, "No Candidates found");

    return 0;

}


vector<PseudoJet> Discriminator::HAnalysis::GetLeptonJets(Analysis::HEvent *const Event)
{

// Lepton Stuff
    vector<float> LeptonEta, LeptonPhi;

    Event->GetLeptons();

//     vector<PseudoJet> LeptonJets = Event->Lepton->LeptonJets;
//     vector<PseudoJet> AntiLeptonJets = Event->Lepton->AntiLeptonJets;

    Event->GetParticles();
    vector<PseudoJet> LeptonJets = Event->Particles->GetLeptonJets();
    vector<PseudoJet> AntiLeptonJets = Event->Particles->GetAntiLeptonJets();

    sort(LeptonJets.begin(), LeptonJets.end(), SortJetByPt());
    sort(AntiLeptonJets.begin(), AntiLeptonJets.end(), SortJetByPt());

    bool HardestLepton = 1;
    for (const auto & LeptonJet : LeptonJets) {

        if (HardestLepton) {

            HLeptonBranch *Lepton = static_cast<HLeptonBranch *>(LeptonBranch->NewEntry());

            Lepton->Pt = LeptonJet.pt();
            Lepton->Eta = LeptonJet.eta();
            Lepton->Phi = LeptonJet.phi_std();
            Lepton->Charge = -1;
            Lepton->Mass = LeptonJet.m();
        }
        HardestLepton = 0;

        LeptonEta.push_back(LeptonJet.eta());
        LeptonPhi.push_back(LeptonJet.phi_std());

    }

    HardestLepton = 1;
    for (const auto & AntiLeptonJet : AntiLeptonJets) {

        if (HardestLepton) {

            HLeptonBranch *Lepton = static_cast<HLeptonBranch *>(LeptonBranch->NewEntry());

            Lepton->Pt = AntiLeptonJet.pt();
            Lepton->Eta = AntiLeptonJet.eta();
            Lepton->Phi = AntiLeptonJet.phi_std();
            Lepton->Charge = 1;
            Lepton->Mass = AntiLeptonJet.m();

        }
        HardestLepton = 0;

        LeptonEta.push_back(AntiLeptonJet.eta());
        LeptonPhi.push_back(AntiLeptonJet.phi_std());

    }

    LeptonJets.insert(LeptonJets.end(), AntiLeptonJets.begin(), AntiLeptonJets.end());

    Print(2, "Number of Leptons", LeptonJets.size());

    return LeptonJets;

}


