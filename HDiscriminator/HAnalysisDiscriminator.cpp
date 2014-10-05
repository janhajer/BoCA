# include "HAnalysisDiscriminator.hh"

HAnalysisDiscriminator::HAnalysisDiscriminator()
{

    Print(0, "Constructor");
    
    ProjectName = "Discriminator";

    LeptonEventCounter = 0;
        
//     ClonesArrays = new HClonesArrayDelphes();
//         
//     Event = new HEventDelphes();
    
//     Debug = 3;
    
    EventNumberMax = 10000;

}

vector<string> HAnalysisDiscriminator::GetStudyNameVector(){
    
    vector<string> StudyNameVector = {"Higgs", "Top", "TwoTop","HiggsTop", "Jet", "Test"};
    //     vector<string> StudyNameVector = {"Top"};
    
    return StudyNameVector;
    
}

void HAnalysisDiscriminator::SetFileVector()
{

    Print(0, "Set File Vector", StudyName);

    if (StudyName != "Higgs") {

        HFileDelphes *Background = new HFileDelphes("pp-bbtt-bblvlv","background");
        Background->Crosssection = 3.215; // pb
        Background->Error = 0.012; // pb
        FileVector.push_back(Background);

    }
    
    HFileDelphes *Even = new HFileDelphes("pp-x0tt-bblvlv","even");
    Even->Crosssection = 0.02079; // pb
    Even->Error = 0.000078; // pb
    FileVector.push_back(Even);

    HFileDelphes *Mix = new HFileDelphes("pp-x0tt-bblvlv","mix");
    Mix->Crosssection = 0.01172; // pb
    Mix->Error = 0.000045; // pb
    FileVector.push_back(Mix);
    
    HFileDelphes *Odd = new HFileDelphes("pp-x0tt-bblvlv","odd");
    Odd->Crosssection = 0.008951; // pb
    Odd->Error = 0.000035; // pb
    FileVector.push_back(Odd);

    int AnalysisSum = FileVector.size();
    Print(0, "Files prepared", AnalysisSum);

}


void HAnalysisDiscriminator::NewFile()
{
    Print(0, "New File");

    CandidateBranch = TreeWriter->NewBranch("Candidate", HCandidateBranch::Class());
    LeptonBranch = TreeWriter->NewBranch("Lepton", HLeptonBranch::Class());
    ConstituentBranch = TreeWriter->NewBranch("Constituent", HConstituentBranch::Class());

    LeptonEventCounter = 0;

}

void HAnalysisDiscriminator::CloseFile()
{
    Print(0, "Close File");
    
}



bool HAnalysisDiscriminator::Analysis()
{

//     Event->GetTaggedJets();
    
    Print(1, "Analysis", StudyName);

    vector<PseudoJet> LeptonVector = Leptons();

    int LeptonSum = LeptonVector.size();

    Print(1, "Number of Leptons", LeptonSum);

    if (LeptonSum < 2) {

        Print(1, "Not enough Leptons", LeptonSum);
        return 0;

    }

    ++LeptonEventCounter;

    // Higgs stuff

//     int HiggsUserIndex = Event->Discriminator->HiggsUserIndex;
//     int TopUserIndex = Event->Discriminator->TopUserIndex;

    int CandidateCounter = 0;

    // Higgs properties

    vector<PseudoJet> CandidateJets = Event->GetHiggsTopCandidates();
    int CandidateSum = CandidateJets.size();

    Print(1, "Number of Candidates", CandidateSum);


    if (CandidateSum < 1) {

        Print(1, "No a Candidates");
        return 0;

    }

    if (StudyName == "Test") {

        CandidateSum = min(CandidateSum, 3);

    }

    bool HasCandidate = 0;

    for (int CandidateNumber = 0; CandidateNumber < CandidateSum; ++CandidateNumber) {

        PseudoJet CandidateJet = CandidateJets[CandidateNumber];

        Print(2, "Candidate", CandidateNumber);

        if (StudyName == "Higgs") {

            if (CandidateJet.user_index() != HiggsUserIndex || CandidateJet.user_index() != CpvHiggsId) {

                Print(2, "Not a Higgs");
                continue;

            }

            if (CandidateCounter > 0) Print(0, "Number of Higgs", CandidateCounter);
            ++CandidateCounter;
            Print(1, "Higgs", CandidateCounter);

        }

        if (StudyName == "Top") {
            
            if (CandidateJet.user_index() != TopUserIndex || CandidateJet.user_index() != TopId) {
                
                Print(2, "Not a Top");
                continue;
                
            }
            
            if (CandidateCounter > 1) Print(0, "Number of Tops", CandidateCounter);            
            ++CandidateCounter;
            Print(1, "Top", CandidateCounter);

        }
        
        if (StudyName == "TwoTop") {
            
            if (CandidateJet.user_index() != 2 * TopUserIndex || CandidateJet.user_index() != 2* TopId){
                
                Print(2, "Not two tops");
                continue;
                
            }
            
            if (CandidateCounter > 0) Print(0, "Number TopPairs", CandidateCounter);            
            ++CandidateCounter;
            Print(1, "TwoTop", CandidateCounter);
            
        }
        
        if (StudyName == "HiggsTop") {
            
            if (CandidateJet.user_index() != HiggsUserIndex + TopUserIndex || CandidateJet.user_index() != CpvHiggsId + TopId){
                
                Print(2, "Not a Top Higgs pair");
                continue;
                
            }
            
            if (CandidateCounter > 0) Print(0, "Number TopHiggsPairs", CandidateCounter);  
            ++CandidateCounter;
            Print(1, "HiggsTop", CandidateCounter);
            
        }

        if (StudyName == "Jet") {

            if (CandidateJet.user_index() == HiggsUserIndex ||CandidateJet.user_index() == CpvHiggsId ||
                CandidateJet.user_index() == TopUserIndex ||CandidateJet.user_index() == TopId ||
                CandidateJet.user_index() == 2 * TopUserIndex ||CandidateJet.user_index() == 2 * TopId ||
                CandidateJet.user_index() == TopUserIndex+ HiggsUserIndex || CandidateJet.user_index() == TopId + CpvHiggsId) {
                
                Print(2, "Not a light jet");
                continue;
                
            }
            
            if (CandidateCounter > 2) Print(0, "Number light jets", CandidateCounter);  
            ++CandidateCounter;            
            Print(1, "Jet", CandidateCounter);

        }

        float CandidateMass = CandidateJet.m();
        float CandidatePt = CandidateJet.perp();

        if (CandidateJet == 0 || CandidateMass <= 0 || CandidatePt <= 0) {

            Print(0, "Illeagal Candidate", CandidateMass);
            continue;

        }

        float Shift = 1;
        float Position2Eta = 2 * Shift;
        float PosDistance = GetDistance(Position2Eta, 0); // Position2Eta

        float CandidateEta = CandidateJet.eta();
        float CandidatePhi = CandidateJet.phi_std();

        float CandidateArea = 0;

//         if (CandidateJet.has_area()) {
// 
//             float CandidateArea = CandidateJet.area();
//             
//             cout << CandidateArea << endl;
// 
// //             if (CandidateArea <= 0) {
// // 
// //                 Print("HAnalysisDiscriminator", "Candidate has no Area");
// //                 continue;
// // 
// //             }
// 
//         } else {
//             
//             cout << 0 << endl;
//             
//         }

        // Get Position of SubJets

        vector<PseudoJet> CandidatePieces = CandidateJet.pieces();
        sort(CandidatePieces.begin(), CandidatePieces.end(), SortJetByMass());
        int PiecesSum = CandidatePieces.size();

        if (!(PiecesSum == 2 /*|| PiecesSum ==3*/)) {

            Print(0, "Wrong Number of SubJets", PiecesSum);
            continue;

        }




        // SubJets

        PseudoJet SubJet1 = CandidatePieces[0];
        PseudoJet SubJet2 = CandidatePieces[1];

        float SubJet1Mass = SubJet1.m();
        float SubJet2Mass = SubJet2.m();

        if (SubJet1Mass <= 0 || SubJet2Mass <= 0) {

            Print(1, "No SubJet Mass");
            continue;

        }

        float SubJet1Pt = SubJet1.perp();
        float SubJet2Pt = SubJet2.perp();

        if (SubJet1Pt <= 0 || SubJet2Pt <= 0) {

            Print(0, "No SubJet Pt");
            continue;

        }


        float SubJetDeltaR = SubJet1.delta_R(SubJet2);

        float SubJet1DeltaR = SubJet1.delta_R(CandidateJet);
        float SubJet2DeltaR = SubJet2.delta_R(CandidateJet);

//         float Asymmetry = SubJet2Pt * SubJetDeltaR / CandidateMass;

        // Get SubJet coordinates in Higgs Jet coordinates

        float SubJet1Eta = SubJet1.eta() - CandidateEta;
        float SubJet1Phi = GetDeltaPhi(SubJet1.phi_std(), CandidatePhi);

        float SubJet2Eta = SubJet2.eta() - CandidateEta;
        float SubJet2Phi = GetDeltaPhi(SubJet2.phi_std(), CandidatePhi);

        // move subjet1 together with subjet2 to origin

        SubJet2Eta -= SubJet1Eta;
        SubJet2Phi -= SubJet1Phi;

        // scale subjet distance to reference value

        float SubJetDistance = GetDistance(SubJet2Eta, SubJet2Phi);
        float SubJetRatio =  PosDistance / SubJetDistance;

        if (SubJetDistance == 0) continue;

        vector<PseudoJet> ConstituentVector = CandidateJet.constituents();
        int ConstituentSum = ConstituentVector.size();



        if (ConstituentSum < 1) {

            Print(0, "Not enough Constituents", ConstituentSum);
            continue;

        }








        // Isolation

        int ClosestLepton = LargeNumber;
        int ClosestPiece = LargeNumber;
        float IsolationDeltaR = LargeNumber;

        float IsolationEta = 0;
        float IsolationPhi = 0;
        float IsolationPt = 0;

        for (int PiecesNumber = 0; PiecesNumber < PiecesSum; ++PiecesNumber) {

            PseudoJet Piece = CandidatePieces[PiecesNumber];

            for (int LeptonNumber = 0; LeptonNumber < LeptonSum; ++LeptonNumber) {

                float DeltaR = LeptonVector[LeptonNumber].delta_R(Piece);
                Print(3, "DeltaR", DeltaR);

                if (DeltaR < IsolationDeltaR) {

                    IsolationDeltaR = DeltaR;
                    ClosestLepton = LeptonNumber;
                    ClosestPiece = PiecesNumber;

                }

            }

        }

        Print(2, "Closest Lepton", ClosestLepton);


        if (IsolationDeltaR != LargeNumber) {

            IsolationEta = LeptonVector[ClosestLepton].eta() - CandidatePieces[ClosestPiece].eta();
            IsolationPhi = GetDeltaPhi(LeptonVector[ClosestLepton].phi(), CandidatePieces[ClosestPiece].phi());
            IsolationPt = LeptonVector[ClosestLepton].pt() / CandidatePieces[ClosestPiece].pt();


        }







        float Theta = atan2(SubJet2Phi, SubJet2Eta);

        float SumInverseEta = 0;
        float SumInversePhi = 0;



        for (int ConstituentNumber = 0; ConstituentNumber < ConstituentSum; ++ConstituentNumber) {

            PseudoJet ConstituentJet = ConstituentVector[ConstituentNumber];
            HConstituentBranch *Constituent = static_cast<HConstituentBranch *>(ConstituentBranch->NewEntry());

            // Get Constituent coordinates in Higgs Jet coordinates

            float ConstEta = ConstituentJet.eta() - CandidateEta;
            float ConstPhi = GetDeltaPhi(ConstituentJet.phi_std(), CandidatePhi);

            // move subjet1 together with constituent to origin

            ConstEta -= SubJet1Eta;
            ConstPhi = GetDeltaPhi(ConstPhi, SubJet1Phi);

            // scale distance to reference value

            ConstEta = ConstEta * SubJetRatio;
            ConstPhi = ConstPhi * SubJetRatio;

            // rotate Constituent according to subjet2

            float ObservableEta = ConstEta * cos(Theta) + ConstPhi * sin(Theta);
            float ObservablePhi = ConstEta * sin(Theta) - ConstPhi * cos(Theta);

            // move subjet2 to (1,0)
            ObservableEta -= Shift;

            float ConstPt = ConstituentJet.perp();


            Constituent->Eta = ObservableEta;
            Constituent->Phi = ObservablePhi;
            Constituent->Pt = ConstPt;


            SumInverseEta += ConstPt / fabs(ObservableEta);
            SumInversePhi += ConstPt / fabs(ObservablePhi);

        }

        float SumEta = CandidatePt / SumInverseEta;
        float SumPhi = CandidatePt / SumInversePhi;





        HasCandidate = 1;

        HCandidateBranch *Candidate = static_cast<HCandidateBranch *>(CandidateBranch->NewEntry());



        Candidate->IsolationEta = IsolationEta ;
        Candidate->IsolationPhi = IsolationPhi;
        Candidate->IsolationPt = IsolationPt;

        Candidate->IsolationDeltaR = LeptonVector[ClosestLepton].delta_R(CandidatePieces[ClosestPiece]);
        Candidate->IsolationAngle = atan2(IsolationPhi, IsolationEta);

        Print(2, "Isolation", Candidate->IsolationDeltaR);


        Candidate->Mass = CandidateMass;
        Candidate->Pt = CandidatePt;
        Candidate->Eta = CandidateEta;
        Candidate->Phi = CandidatePhi;

        Print(2, "Candidate Mass", CandidateMass);

        // Tagging

        int UserIndex = CandidateJet.user_index();

        if (UserIndex == HiggsUserIndex) {
            Candidate->HiggsTag = 1;
            Candidate->TopTag = 0;

        } else if (UserIndex == TopUserIndex) {

            Candidate->TopTag = 1;
            Candidate->HiggsTag = 0;

        } else {

            Candidate->TopTag = 0;
            Candidate->HiggsTag = 0;

        }

        Candidate->SubJet1Mass = SubJet1Mass / CandidateMass;
        Candidate->SubJet2Mass = SubJet2Mass / CandidateMass;
        Candidate->SubJet1Pt = SubJet1Pt / CandidatePt;
        Candidate->SubJet2Pt = SubJet2Pt / CandidatePt;
        Candidate->SubJetsDeltaR = SubJetDeltaR;
        Candidate->SubJet1DeltaR = SubJet1DeltaR / SubJetDeltaR ;
        Candidate->SubJet2DeltaR = SubJet2DeltaR / SubJetDeltaR ;
        Candidate->Area = CandidateArea;



        Candidate->ConstEta = SumEta;
        Candidate->ConstPhi = SumPhi;

        //         Higgs->ConstDeltaR = pow(SumEta,2) + pow(SumPhi,2);
        Candidate->ConstDeltaR = GetDistance(SumEta, SumPhi);
        Candidate->ConstAngle = atan2(SumPhi, SumEta);



        Print(2, "Pull", Candidate->ConstDeltaR);

    }

    if (HasCandidate) return 1;

    Print(1, "No Candidates found");

    return 0;

}


vector<PseudoJet> HAnalysisDiscriminator::Leptons()
{

// Lepton Stuff
    vector<float> LeptonEta, LeptonPhi;

    Event->GetLeptons();
    
    vector<PseudoJet> LeptonVector = Event->Lepton->LeptonJetVector;
    vector<PseudoJet> AntiLeptonVector = Event->Lepton->AntiLeptonJetVector;

//     vector<TLorentzVector> LeptonVector = Event->Leptons->LeptonLorentzVectorVector;
//     vector<TLorentzVector> AntiLeptonVector = Event->Leptons->AntiLeptonLorentzVectorVector;

//     Event->GetParticles();
//     vector<TLorentzVector> LeptonVector = Event->Particle->LeptonVector();
//     vector<TLorentzVector> AntiLeptonVector = Event->Particle->AntiLeptonVector();

    sort(LeptonVector.begin(), LeptonVector.end(), SortJetByPt());
    sort(AntiLeptonVector.begin(), AntiLeptonVector.end(), SortJetByPt());

    int LeptonSum = LeptonVector.size();
//     LeptonSum = min(1,LeptonVector.size());


    for (int LeptonNumber = 0; LeptonNumber < LeptonSum; ++LeptonNumber) {

        if (LeptonNumber == 0) {

            HLeptonBranch *Lepton = static_cast<HLeptonBranch *>(LeptonBranch->NewEntry());

            Lepton->Pt = LeptonVector[LeptonNumber].pt();
            Lepton->Eta = LeptonVector[LeptonNumber].eta();
            Lepton->Phi = LeptonVector[LeptonNumber].phi_std();
            Lepton->Charge = -1;
            Lepton->Mass = LeptonVector[LeptonNumber].m();

        }

        LeptonEta.push_back(LeptonVector[LeptonNumber].eta());
        LeptonPhi.push_back(LeptonVector[LeptonNumber].phi_std());

    }

//     LeptonSum = min(1,AntiLeptonVector.size());
    LeptonSum = AntiLeptonVector.size();
    for (int LeptonNumber = 0; LeptonNumber < LeptonSum; ++LeptonNumber) {

        if (LeptonNumber == 0) {

            HLeptonBranch *Lepton = static_cast<HLeptonBranch *>(LeptonBranch->NewEntry());

            Lepton->Pt = AntiLeptonVector[LeptonNumber].pt();
            Lepton->Eta = AntiLeptonVector[LeptonNumber].eta();
            Lepton->Phi = AntiLeptonVector[LeptonNumber].phi_std();
            Lepton->Charge = 1;
            Lepton->Mass = AntiLeptonVector[LeptonNumber].m();

        }

        LeptonEta.push_back(AntiLeptonVector[LeptonNumber].eta());
        LeptonPhi.push_back(AntiLeptonVector[LeptonNumber].phi_std());

    }

    LeptonVector.insert(LeptonVector.end(), AntiLeptonVector.begin(), AntiLeptonVector.end());

    return (LeptonVector);

}

