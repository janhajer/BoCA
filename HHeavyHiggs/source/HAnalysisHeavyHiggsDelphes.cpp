# include "HAnalysisHeavyHiggsDelphes.hh"

HAnalysisHeavyHiggsDelphes::HAnalysisHeavyHiggsDelphes()
{

    Print(0, "Constructor");

    ProjectName = "HeavyHiggs";

    EventNumberMax = 10000;

    Cut = 0;
   

}

vector<string> HAnalysisHeavyHiggsDelphes::GetStudyNameVector()
{

    vector<string> StudyNameVector = {"Signal", "Background", "Test"};
//     vector<string> StudyNameVector = {"Background"};
    

    return StudyNameVector;

}

void HAnalysisHeavyHiggsDelphes::SetFileVector()
{

    Print(0, "Fill Analysis Vector", AnalysisName);


//     if (AnalysisName != "Signal") {

    FileVector.push_back(new HFileFolder("5k_Pt20")); // FIXME we need this file

//     }

    FileVector.push_back(new HFileFolder("Signal_5f"));
//     FileVector.push_back(new HFileFolder("500GeV"));
//     FileVector.push_back(new HFileFolder("600GeV"));
//     FileVector.push_back(new HFileFolder("700GeV"));
//     FileVector.push_back(new HFileFolder("800GeV"));
//     FileVector.push_back(new HFileFolder("900GeV"));
//     FileVector.push_back(new HFileFolder("1000GeV"));

    FileVector.front()->BasePath = "~/Projects/HeavyHiggs/Mass/";
//     FileVector.front()->BasePath = "~/Dropbox/Projects/HeavyHiggs/Simulation/";
    FileVector.front()->FileSuffix = "_Delphes.root";
    FileVector.front()->Snowmass = 1;

//     FileVector.push_back(new HFileDelphes("pp-bbtt-4f", "background"));

    Print(0, "Files prepared", FileVector.size());

}

void HAnalysisHeavyHiggsDelphes::NewFile()
{

    Print(0, "New File");

    HeavyHiggsBranch = TreeWriter->NewBranch("HeavyHiggs", HHeavyHiggsBranch::Class());

    EventCounter = 0;
    DeltaEtaCounter = 0;
    BMassCounter = 0;
    TMassCounter = 0;
    JetCounter = 0;
    Jet2Counter = 0;

}

void HAnalysisHeavyHiggsDelphes::CloseFile()
{
    Print(0, "Close File");

    if (Cut) {

        Print(0, "EventCounter", EventCounter);
        Print(0, "JetCounter", JetCounter);
        Print(0, "DeltaEtaCounter", DeltaEtaCounter);
        Print(0, "BMassCounter", BMassCounter);
        Print(0, "Jet2Counter", Jet2Counter);
        Print(0, "TMassCounter", TMassCounter);

    }

}

bool HAnalysisHeavyHiggsDelphes::Analysis()
{

    Print(1, "Analysis", AnalysisName);

    bool Success = 0;
 
    Event->GetLeptons();

    if (StudyName == "Signal") Success = Signal();
    if (StudyName == "Background") Success = Background();
    if (StudyName == "Test")  Success = Test();

    return Success;

}

// bool HAnalysisHeavyHiggsDelphes::JetIsBottom(const PseudoJet &Jet)
// {
// 
//     return abs(Jet.user_index()) == BottomId;
// 
// }


bool HAnalysisHeavyHiggsDelphes::Signal()
{

    Print(1, "Signal");

    Event->GetTaggedJets();

    vector<PseudoJet> JetVector = Event->Jets->JetVector;

    vector<PseudoJet> BottomJetVector;

    for (unsigned JetNumber = 0; JetNumber < JetVector.size(); ++JetNumber) {

        if (abs(JetVector[JetNumber].user_index()) == BottomId) BottomJetVector.push_back(JetVector[JetNumber]);

    }

    if (BottomJetVector.size() < 2)  return 0;


    sort(BottomJetVector.begin(), BottomJetVector.end(), SortJetByEta());

    PseudoJet FrontJet = BottomJetVector.front();
    PseudoJet BackJet = BottomJetVector.back();

    FillBranch(FrontJet, BackJet);

    return 1;

}


bool HAnalysisHeavyHiggsDelphes::Background()
{

    Print(1, "Background");

    Event->GetTaggedJets();

    vector<PseudoJet> JetVector = Event->Jets->JetVector;
    
    if (JetVector.size()<1) return 0;

    vector<PseudoJet> TopJetVector, BottomJetVector;

    for (unsigned JetNumber = 0; JetNumber < JetVector.size(); ++JetNumber) {

        if (abs(JetVector[JetNumber].user_index()) == BottomId) BottomJetVector.push_back(JetVector[JetNumber]);
        if (abs(JetVector[JetNumber].user_index()) == TopId) TopJetVector.push_back(JetVector[JetNumber]);

    }

    bool Success = 0;

    if (TopJetVector.size() > 1) {

        Success = 1;

        for (unsigned TopJetNumber = 0; TopJetNumber < TopJetVector.size(); ++TopJetNumber) {

            for (unsigned SecondTopJetNumber = 0; SecondTopJetNumber < TopJetNumber; ++SecondTopJetNumber) {

                PseudoJet FrontJet, BackJet;

                if (TopJetVector[TopJetNumber].eta() > TopJetVector[SecondTopJetNumber].eta()) {

                    FrontJet = TopJetVector[TopJetNumber];
                    BackJet = TopJetVector[SecondTopJetNumber];

                } else {

                    FrontJet = TopJetVector[SecondTopJetNumber];
                    BackJet = TopJetVector[TopJetNumber];

                }

                FillBranch(FrontJet, BackJet);

            }

        }

    }

    if (TopJetVector.size() > 0 && BottomJetVector.size() > 0) {

        Success = 1;

        for (unsigned TopJetNumber = 0; TopJetNumber < TopJetVector.size(); ++TopJetNumber) {

            for (unsigned BottomJetNumber = 0; BottomJetNumber < BottomJetVector.size(); ++BottomJetNumber) {

                PseudoJet FrontJet, BackJet;

                if (TopJetVector[TopJetNumber].eta() > TopJetVector[BottomJetNumber].eta()) {

                    FrontJet = TopJetVector[TopJetNumber];
                    BackJet = TopJetVector[BottomJetNumber];

                } else {

                    FrontJet = TopJetVector[BottomJetNumber];
                    BackJet = TopJetVector[TopJetNumber];

                }

                FillBranch(FrontJet, BackJet);

            }

        }

    }

    return Success;

}

bool HAnalysisHeavyHiggsDelphes::Test()
{

    Print(1, "Test");

    Event->GetJets();

    vector<PseudoJet> BottomJetVector = Event->Jets->BottomJetVector;

    if (BottomJetVector.size() < 2) return 0;

    PseudoJet FrontJet, BackJet;

    for (unsigned BottomJetNumber = 0; BottomJetNumber < BottomJetVector.size(); ++BottomJetNumber) {

        for (unsigned SecondBottomJetNumber = 0; SecondBottomJetNumber < BottomJetNumber; ++SecondBottomJetNumber) {

            if (BottomJetVector[BottomJetNumber].eta() > BottomJetVector[SecondBottomJetNumber].eta()) {

                FrontJet = BottomJetVector[BottomJetNumber];
                BackJet = BottomJetVector[SecondBottomJetNumber];

            } else {

                FrontJet = BottomJetVector[SecondBottomJetNumber];
                BackJet = BottomJetVector[BottomJetNumber];

            }

            FillBranch(FrontJet, BackJet);

        }

    }

    return 1;

}
void HAnalysisHeavyHiggsDelphes::FillBranch(PseudoJet FrontJet, PseudoJet BackJet)
{

    float FrontPt = FrontJet.pt();
    float FrontEta = FrontJet.eta();
    float FrontPhi = FrontJet.phi_std();

    float BackEta = BackJet.eta();
    float BackPt = BackJet.pt();
    float BackPhi = BackJet.phi_std();

    float InvMass = (FrontJet + BackJet).m();
    float DeltaEta = FrontEta - BackEta;
    float SumEta = FrontEta + BackEta;
    float DeltaPhi = FrontJet.delta_phi_to(BackJet);
    float SumPhi = FrontPhi + BackPhi; // FIXME constrain this
    float DeltaPt = FrontPt - BackPt;

    float Isolation = min(Leptons(FrontJet), Leptons(BackJet));

    HHeavyHiggsBranch *HeavyHiggs = static_cast<HHeavyHiggsBranch *>(HeavyHiggsBranch->NewEntry());

    HeavyHiggs->BottomEta1 = FrontEta;
    HeavyHiggs->BottomEta2 = BackEta;
    HeavyHiggs->BottomDeltaEta = DeltaEta;
    HeavyHiggs->BottomSumEta = SumEta;

    HeavyHiggs->BottomPhi1 = FrontPhi;
    HeavyHiggs->BottomPhi2 = BackPhi;
    HeavyHiggs->BottomDeltaPhi = DeltaPhi;
    HeavyHiggs->BottomSumPhi = SumPhi;

    HeavyHiggs->BottomPt1 = FrontPt;
    HeavyHiggs->BottomPt2 = BackPt;
    HeavyHiggs->BottomInvMass = InvMass;
    HeavyHiggs->BottomDeltaPt = DeltaPt;

    HeavyHiggs->BTag = Event->Jets->BottomJetVector.size();
    HeavyHiggs->JetNumber = Event->Jets->JetVector.size();
    HeavyHiggs->Isolation = Isolation;

}


float HAnalysisHeavyHiggsDelphes::Leptons(PseudoJet Jet)
{

    float Isolation;

    vector<PseudoJet> LeptonVector = Event->Lepton->GetLeptonJetVector();

    for (unsigned LeptonNumber = 0; LeptonNumber < LeptonVector.size(); ++LeptonNumber) {

        Isolation = Jet.delta_R(LeptonVector[LeptonNumber]);

    }

    return Isolation;

}


