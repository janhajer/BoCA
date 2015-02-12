# include "HAnalysisHeavyHiggsDelphes.hh"

hheavyhiggs::HAnalysisHeavyHiggsDelphes::HAnalysisHeavyHiggsDelphes()
{

    Print(HNotification, "Constructor");

}

HStrings hheavyhiggs::HAnalysisHeavyHiggsDelphes::GetStudyNameVector()
{

//     HStrings StudyNameVector = {"Signal", "Background", "Test"};
    HStrings StudyNameVector = {"Signal"};


    return StudyNameVector;

}

std::vector<hanalysis::HFile*> hheavyhiggs::HAnalysisHeavyHiggsDelphes::GetFiles(const std::string &Study)
{

    Print(HNotification, "Fill Analysis Vector", Study);

    std::vector<hanalysis::HFile*> NewFiles;

    NewFiles.push_back(new hanalysis::HFile("BG_ttbb"));
    NewFiles.push_back(new hanalysis::HFile("hcpbb_ttbb"));

    NewFiles.front()->SetBasePath("~/Projects/HeavyHiggs/Mass/");
//     FileVector.front()->BasePath = "~/Dropbox/Projects/HeavyHiggs/Simulation/";
    NewFiles.front()->SetFileSuffix(".root");
    NewFiles.front()->SetSnowMass(true);

//     FileVector.push_back(new HFile("pp-bbtt-4f", "background"));

    Print(HNotification, "Files prepared");

    return NewFiles;

}

void hheavyhiggs::HAnalysisHeavyHiggsDelphes::NewBranches(ExRootTreeWriter *NewTreeWriter)
{

    Print(HNotification, "New File");

    HeavyHiggsBranch = NewTreeWriter->NewBranch("HeavyHiggs", HBranchHeavyHiggs::Class());

    EventCounter = 0;
    DeltaRapCounter = 0;
    BMassCounter = 0;
    TMassCounter = 0;
    JetCounter = 0;
    Jet2Counter = 0;

}

void hheavyhiggs::HAnalysisHeavyHiggsDelphes::CloseFile()
{
    Print(HNotification, "Close File");

//     if (Cut) {

        Print(HNotification, "EventCounter", EventCounter);
        Print(HNotification, "JetCounter", JetCounter);
        Print(HNotification, "DeltaRapCounter", DeltaRapCounter);
        Print(HNotification, "BMassCounter", BMassCounter);
        Print(HNotification, "Jet2Counter", Jet2Counter);
        Print(HNotification, "TMassCounter", TMassCounter);

//     }

}

bool hheavyhiggs::HAnalysisHeavyHiggsDelphes::Analysis(hanalysis::HEvent &Event, const std::string &Name)
{

    Print(HInformation, "Analysis");

    bool Success = 0;

//     Event.GetLeptonsM()->GetLeptons();

    if (Name == "Signal") Success = Signal(Event);
    if (Name == "Background") Success = Background(Event);
    if (Name == "Test")  Success = Test(Event);

    return Success;

}

// class hheavyhiggs::HHeavyHiggsJetTag : public hanalysis::HJetTag {
//
//     int GetBranchId(int, int);
//
// };

// int hheavyhiggs::HHeavyHiggsJetTag::GetBranchId(const int ParticleId, int BranchId)
// {
//
//     Print(HInformation, "Get Mother Id");
//
//     if (RadiationParticles.find(std::abs(ParticleId)) != end(RadiationParticles) && HeavyParticles.find(std::abs(BranchId)) == end(HeavyParticles)) {
//         BranchId = IsrId;
//     } else if (std::abs(ParticleId) == BottomId && (std::abs(BranchId) != TopId && std::abs(BranchId) != CpvHiggsId)) {
//         BranchId = ParticleId;
//     } else if (std::abs(ParticleId) == TopId || std::abs(ParticleId) == CpvHiggsId) {
//         BranchId = ParticleId;
//     }
//
//     Print(HDetailed, "Mother Id", BranchId);
//
//
//     return BranchId;
//
// }

// bool HAnalysisHeavyHiggsDelphes::JetIsBottom(const fastjet::PseudoJet &Jet)
// {
//
//     return std::abs(Jet.user_index()) == BottomId;
//
// }


bool hheavyhiggs::HAnalysisHeavyHiggsDelphes::Signal(hanalysis::HEvent &Event)
{

    Print(HInformation, "Signal");

    hanalysis::HJetTag  JetTag;
//     Event.GetTaggedJets(HeavyHiggsJetTag);

    HJets JetVector = Event.GetJets()->GetTaggedJets(JetTag);

    HJets BottomJetVector;

    for (unsigned JetNumber = 0; JetNumber < JetVector.size(); ++JetNumber) {

        if (std::abs(JetVector[JetNumber].user_index()) == BottomId) BottomJetVector.push_back(JetVector[JetNumber]);

    }

    if (BottomJetVector.size() < 2)  return 0;

    Print(HError,"Signal Bottom",BottomJetVector.size());

    std::sort(BottomJetVector.begin(), BottomJetVector.end(), SortJetByRap());

    fastjet::PseudoJet FrontJet = BottomJetVector.front();
    fastjet::PseudoJet BackJet = BottomJetVector.back();

    FillBranch(Event,FrontJet, BackJet);

    return 1;

}


bool hheavyhiggs::HAnalysisHeavyHiggsDelphes::Background(hanalysis::HEvent &Event)
{

    Print(HInformation, "Background");

    hanalysis::HJetTag  HeavyHiggsJetTag;
//     Event.GetTaggedJets(HeavyHiggsJetTag);

    HJets JetVector = Event.GetJets()->GetTaggedJets(HeavyHiggsJetTag);

    if (JetVector.size()<1) return 0;

    HJets TopJetVector, BottomJetVector;

    for (unsigned JetNumber = 0; JetNumber < JetVector.size(); ++JetNumber) {

        if (std::abs(JetVector[JetNumber].user_index()) == BottomId) BottomJetVector.push_back(JetVector[JetNumber]);
        if (std::abs(JetVector[JetNumber].user_index()) == TopId) TopJetVector.push_back(JetVector[JetNumber]);

    }

    bool Success = 0;

    if (TopJetVector.size() > 1) {

        Success = 1;

        for (unsigned TopJetNumber = 0; TopJetNumber < TopJetVector.size(); ++TopJetNumber) {

            for (unsigned SecondTopJetNumber = 0; SecondTopJetNumber < TopJetNumber; ++SecondTopJetNumber) {

                fastjet::PseudoJet FrontJet, BackJet;

                if (TopJetVector[TopJetNumber].rap() > TopJetVector[SecondTopJetNumber].rap()) {

                    FrontJet = TopJetVector[TopJetNumber];
                    BackJet = TopJetVector[SecondTopJetNumber];

                } else {

                    FrontJet = TopJetVector[SecondTopJetNumber];
                    BackJet = TopJetVector[TopJetNumber];

                }

                FillBranch(Event,FrontJet, BackJet);

            }

        }

    }

    if (TopJetVector.size() > 0 && BottomJetVector.size() > 0) {

        Success = 1;

        for (unsigned TopJetNumber = 0; TopJetNumber < TopJetVector.size(); ++TopJetNumber) {

            for (unsigned BottomJetNumber = 0; BottomJetNumber < BottomJetVector.size(); ++BottomJetNumber) {

                fastjet::PseudoJet FrontJet, BackJet;

                if (TopJetVector[TopJetNumber].rap() > TopJetVector[BottomJetNumber].rap()) {

                    FrontJet = TopJetVector[TopJetNumber];
                    BackJet = BottomJetVector[BottomJetNumber];

                } else {

                    FrontJet = BottomJetVector[BottomJetNumber];
                    BackJet = TopJetVector[TopJetNumber];

                }

                FillBranch(Event,FrontJet, BackJet);

            }

        }

    }

    return Success;

}

bool hheavyhiggs::HAnalysisHeavyHiggsDelphes::Test(hanalysis::HEvent &Event)
{

    Print(HInformation, "Test");

//     Event.GetJets();

    HJets BottomJetVector = Event.GetJets()->GetBottomJets();

    if (BottomJetVector.size() < 2) return 0;

    fastjet::PseudoJet FrontJet, BackJet;

    for (unsigned BottomJetNumber = 0; BottomJetNumber < BottomJetVector.size(); ++BottomJetNumber) {

        for (unsigned SecondBottomJetNumber = 0; SecondBottomJetNumber < BottomJetNumber; ++SecondBottomJetNumber) {

            if (BottomJetVector[BottomJetNumber].rap() > BottomJetVector[SecondBottomJetNumber].rap()) {

                FrontJet = BottomJetVector[BottomJetNumber];
                BackJet = BottomJetVector[SecondBottomJetNumber];

            } else {

                FrontJet = BottomJetVector[SecondBottomJetNumber];
                BackJet = BottomJetVector[BottomJetNumber];

            }

            FillBranch(Event,FrontJet, BackJet);

        }

    }

    return 1;

}

void hheavyhiggs::HAnalysisHeavyHiggsDelphes::FillBranch(hanalysis::HEvent &Event,fastjet::PseudoJet FrontJet, fastjet::PseudoJet BackJet)
{

    float FrontPt = FrontJet.pt();
    float FrontRap = FrontJet.rap();
    float FrontPhi = FrontJet.phi_std();

    float BackRap = BackJet.rap();
    float BackPt = BackJet.pt();
    float BackPhi = BackJet.phi_std();

    float InvMass = (FrontJet + BackJet).m();
    float DeltaRap = FrontRap - BackRap;
    float SumRap = FrontRap + BackRap;
    float DeltaPhi = FrontJet.delta_phi_to(BackJet);
    float SumPhi = FrontPhi + BackPhi; // FIXME constrain this
    float DeltaPt = FrontPt - BackPt;

    float Isolation = std::min(Leptons(Event,FrontJet), Leptons(Event,BackJet));

    HBranchHeavyHiggs *HeavyHiggs = static_cast<HBranchHeavyHiggs *>(HeavyHiggsBranch->NewEntry());

    HeavyHiggs->BottomRap1 = FrontRap;
    HeavyHiggs->BottomRap2 = BackRap;
    HeavyHiggs->BottomDeltaRap = DeltaRap;
    HeavyHiggs->BottomSumRap = SumRap;

    HeavyHiggs->BottomPhi1 = FrontPhi;
    HeavyHiggs->BottomPhi2 = BackPhi;
    HeavyHiggs->BottomDeltaPhi = DeltaPhi;
    HeavyHiggs->BottomSumPhi = SumPhi;

    HeavyHiggs->BottomPt1 = FrontPt;
    HeavyHiggs->BottomPt2 = BackPt;
    HeavyHiggs->BottomInvMass = InvMass;
    HeavyHiggs->BottomDeltaPt = DeltaPt;

    HeavyHiggs->BTag = Event.GetJets()->GetBottomJets().size();
    HeavyHiggs->JetNumber = Event.GetJets()->GetJets().size();
    HeavyHiggs->Isolation = Isolation;

}


float hheavyhiggs::HAnalysisHeavyHiggsDelphes::Leptons(hanalysis::HEvent &Event,fastjet::PseudoJet Jet)
{

    float Isolation;

    HJets LeptonVector = Event.GetLeptons()->GetLeptonJets();

    for (unsigned LeptonNumber = 0; LeptonNumber < LeptonVector.size(); ++LeptonNumber) {

        Isolation = Jet.delta_R(LeptonVector[LeptonNumber]);

    }

    return Isolation;

}



