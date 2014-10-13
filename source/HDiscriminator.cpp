# include "HDiscriminator.hh"

HDiscriminator::HDiscriminator()
{

    Print(1, "Constructor");

//     DebugLevel = 5;

}

HDiscriminator::~HDiscriminator()
{

    Print(1, "Destructor");

}

void HDiscriminator::NewFile()
{

    Print(2, "New Analysis");

}

void HDiscriminator::CloseFile()
{

    Print(2, "Clean Analysis");

}



void HDiscriminator::NewEvent()
{

    Print(2, "New Event");

}

// vector<PseudoJet> HDiscriminator::GetTaggedCandidateJets(
//     vector<PseudoJet> NewEFlowJetVector,
//     vector<PseudoJet> NewHiggsParticleVector,
//     vector<PseudoJet> NewTopParticleVector)
// {
//
//     Print(2, "Get Tagged Candidate Jets");
//
//     EFlowJetVector = NewEFlowJetVector;
//     HiggsParticleVector = NewHiggsParticleVector;
//     TopParticleVector = NewTopParticleVector;
//
//     const vector<PseudoJet> Jets;
//
//     if (EFlowJetSum() == 0) {
//
//         Print(2, "No EFlow Jets");
//         return Jets;
//
//     }
//
//     if (!HiggsParticleChecker()) return Jets;
//
//     if (!TopParticleChecker()) return Jets;
//
//     HiggsParticleChecker();
//     TopParticleChecker();
//
//     const int FatJetGoal = 3;
//     int FatJetPseudoGoal = FatJetGoal - 1;
//
//     while (FatJetSum() < FatJetGoal) {
//
//         ++FatJetPseudoGoal;
//         if (!GetSuperFatJetVector(FatJetPseudoGoal)) {
//
//             return Jets;
//
//             Print(1, "Not enough valid jets");
//
//         }
//
//         GetMassDropVector();
//
//     }
//
//     FatJetTagger();
//
//     sort(FatJetVector.begin(), FatJetVector.end(), SortJetByMass());
//
//     return FatJetVector;
//
// }

vector<PseudoJet> HDiscriminator::GetCandidateJets(const vector<PseudoJet> &EFlowJetVector)
{

    Print(2, "Get Tagged Candidate Jets",EFlowJetVector.size());

    const vector<PseudoJet> Jets;

    if (EFlowJetVector.size() == 0) {

        Print(2, "No EFlow Jets");
        return Jets;

    }

//     const int FatJetGoal = 3;
//     int FatJetPseudoGoal = FatJetGoal - 1;
//
//     while (FatJetSum() < FatJetGoal) {
//
//         ++FatJetPseudoGoal;
//         if (!GetSuperFatJetVector(FatJetPseudoGoal)) {
//
//             return Jets;
//
//             Print(1, "Not enough valid jets");
//
//         }
//
//         GetMassDropVector();
//
//     }


    float PtSum;

    for (const auto & EFlowJet : EFlowJetVector) {

        PtSum += EFlowJet.pt();

    }

    const float DeltaR = 1000. / PtSum;

    const fastjet::JetAlgorithm FatJetAlgorithm = fastjet::cambridge_algorithm;
    const fastjet::JetDefinition FatJetDefinition(FatJetAlgorithm, DeltaR);
    const vector<PseudoJet> FatJetVector = GetFatJetVector(EFlowJetVector, FatJetDefinition);

    vector<PseudoJet> MassDropVector = GetMassDropVector(FatJetVector);
    //     vector<PseudoJet> MassDropVector = FatJetVector;
    
    MassDropVector.erase(std::remove_if(MassDropVector.begin(), MassDropVector.end(), JetIsBad), MassDropVector.end());


    sort(MassDropVector.begin(), MassDropVector.end(), SortJetByMass());
    MassDropVector = GetFatJetTag(MassDropVector);

    return MassDropVector;
    
}


// bool HDiscriminator::HiggsParticleChecker()
// {
//
//     Print(2, "Higgs Particle Checker");
//
//     Print(4, "Higgs Particles", HiggsParticleSum());
//
//     if (HiggsParticleSum() != 1) {
//
//         if (HiggsParticleSum() == 0) {
//
//             Print(3, "No Higgs Particles", HiggsParticleSum());
//
//             return 1;
//
//         } else if (HiggsParticleSum() > 1) {
//
//             Print(1, "Number of Higgs Particles", HiggsParticleSum());
//
//             return 0;
//
//         }
//
//     }
//
//     HasHiggs = 1;
//
//     return 1;
//
// }


// bool HDiscriminator::TopParticleChecker()
// {
//
//     Print(2, "Top Particle Checker");
//
//
//     if (TopParticleSum() != 2) {
//
//         Print(1, "Number of Top Particles", TopParticleSum());
//
//         return 0;
//
//     }
//
//     return 1;
//
// }

// bool HDiscriminator::GetSuperFatJetVector(int FatJetGoal)
// {
//
//     Print(2, "Get Super Fat Jet Vector", FatJetGoal);
//
//     float DeltaR = 0;
//     TestFatJetVector(DeltaR);
//     int FatJetSumMax = FatJetSum();
//
//     Print(3, "MaxNumber of Jets", FatJetSumMax);
//
//     if (FatJetSumMax < FatJetGoal) return 0;
//
//
//     while (FatJetSum() > FatJetGoal) {
//
//         DeltaR += 1;
//
//         TestFatJetVector(DeltaR);
//
//     }
//
//
//     Print(3, "Second Loop");
//
//     DeltaR -= 1;
//     TestFatJetVector(DeltaR);
//
//     while (FatJetSum() > FatJetGoal) {
//
//         DeltaR += .1;
//
//         TestFatJetVector(DeltaR);
//
//     }
//
//
//     Print(3, "Third Loop");
//
//     DeltaR -= .1;
//     TestFatJetVector(DeltaR);
//
//     while (FatJetSum() > FatJetGoal) {
//
//         DeltaR += .01;
//
//         TestFatJetVector(DeltaR);
//
//     }
//
//     GetFatJetVector(DeltaR);
//
//     return 1;
//
// }


// void HDiscriminator::TestFatJetVector(float DeltaR)
// {
//
//     Print(4, "Test Fat Jet with DeltaR", DeltaR);
//
//     JetDefinition FatJetDefinition(fastjet::cambridge_algorithm, DeltaR);
//     ClusterSequence FatJetClusterSequence(EFlowJetVector, FatJetDefinition);
//
//     const float FatJetPtMin = 0;
//     FatJetVector = FatJetClusterSequence.inclusive_jets(FatJetPtMin);
//
//     Print(4, "Number of Fat Jets", FatJetSum());
//
// }

// void HDiscriminator::GetFatJetVector(float DeltaR)
// {
//
//     Print(2, "Get Fat Jet Vector", DeltaR);
//
//     JetDefinition FatJetDefinition(fastjet::cambridge_algorithm, DeltaR);
//     ClusterSequence *FatJetClusterSequence = new ClusterSequence(EFlowJetVector, FatJetDefinition);
//
//     const float FatJetPtMin = 0;
//     FatJetVector = FatJetClusterSequence->inclusive_jets(FatJetPtMin);
//
//     Print(3, "Number of Fat Jets", FatJetSum());
//
//     if (FatJetSum() > 0) FatJetClusterSequence->delete_self_when_unused();
//
// }


// bool HDiscriminator::JetIsBad(const PseudoJet &Jet)
// {
//
//     if (abs(Jet.m()) <= 1) {
//
// //         Print(2, "Fat Jet Mass", Jet.m());
//
//         return 1;
//
//     }
//
//
//     if (Jet.pieces().size() != 2) {
//
// //         Print(2, "Pieces Sum", Jet.pieces().size());
//
//         return 1;
//
//     }
//
// //     if (!Jet.has_structure()) {
// //
// // //         Print(2, "PseudoJet has no structure");
// //
// //         return 1;
// //
// //     }
//
//
//     return 0;
//
// }

// bool HDiscriminator::JetIsBad2(const PseudoJet &Jet)
// {
//
//
//     if (Jet.has_structure()) return 0;
//
// //             Print(2, "PseudoJet has no structure");
//
//     return 1;
//
// }


// void HDiscriminator::GetMassDropVector()
// {
//
//     Print(2, "Get Mass Drop Jet Vector");
//
//
//
//     for (int FatJetNumber = 0; FatJetNumber < FatJetSum(); ++FatJetNumber) {
//
//         FatJetVector[FatJetNumber] = GetMassDropJet(FatJetVector[FatJetNumber]);
//
//     }
//
//     FatJetVector.erase(std::remove_if(FatJetVector.begin(), FatJetVector.end(), JetIsBad2), FatJetVector.end());
//
//     FatJetVector.erase(std::remove_if(FatJetVector.begin(), FatJetVector.end(), JetIsBad), FatJetVector.end());
//
//
// //     for (int FatJetNumber = 0; FatJetNumber < FatJetSum(); ++FatJetNumber) {
// //
// //         PseudoJet FatJet = FatJetVector[FatJetNumber];
// //
// // //         if (FatJet.m() <= 10) {
// // //
// // //             Print(2, "Fat Jet Mass", FatJet.m());
// // //
// // //             FatJetVector.erase(FatJetVector.begin() + FatJetNumber);
// // //
// // //             continue;
// // //
// // //         }
// //
// //         vector<PseudoJet> FatJetPieces = FatJet.pieces();
// //
// // //         int PiecesSum = FatJetPieces.size();
// // //
// // //         if (PiecesSum != 2) {
// // //
// // //             Print(2, "Pieces Sum", PiecesSum);
// // //
// // //             FatJetVector.erase(FatJetVector.begin() + FatJetNumber);
// // //             continue;
// // //
// // //         }
// //
// //         FatJet = GetMassDropJet(FatJet);
// //
// //         if (FatJet.has_structure()) {
// //
// //             FatJetVector[FatJetNumber] = FatJet;
// //
// //         } else {
// //
// //             FatJetVector.erase(FatJetVector.begin() + FatJetNumber);
// //
// //             Print(2, "PseudoJet has no structure");
// //             continue;
// //
// //         }
// //
// //     }
//
// }
//
// PseudoJet HDiscriminator::GetMassDropJet(PseudoJet FatJet)
// {
//
//     Print(3, "Get Mass Drop Jet");
//
// //     MassDropMin = Jing: 0.667; fastjet: 0.667; Paper: 0.67
//     const float MassDropMin = 0.667;
// //     AsymmetryCut = Jing: 0.09; fastjet: 0.09; paper: 0.09
//     const float AsymmetryCut = 0.09;
//     MassDropTagger FatJetMassDroppTagger(MassDropMin, AsymmetryCut);
//     PseudoJet MassDropJet = FatJetMassDroppTagger(FatJet);
//
//     return (MassDropJet);
//
// }


// void HDiscriminator::HiggsTagger()
// {
//
//     Print(2, "Higgs Tagger");
//
//     PseudoJet HiggsParticle = HiggsParticleVector[0];
//
//     Print(4, "EFlow Jets", EFlowJetSum());
//     int HiggsParticlePosition;
//
//     float HiggsJetParticleDistance = LargeNumber;
//     for (int EFlowNumber = 0; EFlowNumber < EFlowJetSum(); ++EFlowNumber) {
//
//         PseudoJet EFlowJet = EFlowJetVector[EFlowNumber];
//
//         float EFlowHiggsDistance = EFlowJet.delta_R(HiggsParticle);
//
//         if (EFlowHiggsDistance < HiggsJetParticleDistance) {
//
//             HiggsJetParticleDistance = EFlowHiggsDistance;
//             HiggsParticlePosition = EFlowNumber;
//
//         }
//
//     }
//
//     EFlowJetVector[HiggsParticlePosition].set_user_index(HiggsUserIndex);
//
//     Print(2, "Higgs Tagged", HiggsParticlePosition);
//
// }
//
//
// void HDiscriminator::TopTagger()
// {
//
//     Print(2, "Top Tagger");
//
//     for (int TopParticleNumber = 0; TopParticleNumber < TopParticleSum(); ++TopParticleNumber) {
//
//         PseudoJet TopParticle = TopParticleVector[TopParticleNumber];
//
//         int TopParticlePosition;
//
//         float TopJetParticleDistance = LargeNumber;
//         for (int EFlowNumber = 0; EFlowNumber < EFlowJetSum(); ++EFlowNumber) {
//
//             PseudoJet EFlowJet = EFlowJetVector[EFlowNumber];
//
//             float EFlowHiggsDistance = EFlowJet.delta_R(TopParticle);
//
//             if (EFlowHiggsDistance < TopJetParticleDistance) {
//
//                 TopJetParticleDistance = EFlowHiggsDistance;
//                 TopParticlePosition = EFlowNumber;
//
//             }
//
//         }
//
//         EFlowJetVector[TopParticlePosition].set_user_index(TopUserIndex);
//
//         Print(2, "Top Tagged", TopParticlePosition);
//
//     }
//
// }
//
// void HDiscriminator::GetFatJetTag()
// {
//
//     Print(2, "Get Fat Jet Tag");
//
//     int ConstituentId;
//     float ConstituentPt;
//     vector<PseudoJet> ConstituentsVector;
//     HJetInfo JetInfo;
//
//     for (int FatJetNumber = 0; FatJetNumber < FatJetSum(); ++FatJetNumber) {
//
//         Print(2, "Fat Jet", FatJetNumber);
//
//         ConstituentsVector = FatJetVector[FatJetNumber].constituents();
//
//         for (unsigned ConstituentsNumber = 0; ConstituentsNumber < ConstituentsVector.size(); ++ConstituentsNumber) {
//
//             ConstituentId = ConstituentsVector[ConstituentsNumber].user_index();
//             ConstituentPt = ConstituentsVector[ConstituentsNumber].pt();
//
//             JetInfo.AddConstituent(ConstituentId, ConstituentPt);
//
//         }
//
//         FatJetVector[FatJetNumber].set_user_info(new HJetInfo(JetInfo));
//
//         FatJetVector[FatJetNumber].set_user_index(FatJetVector[FatJetNumber].user_info<HJetInfo>().GetMaximalId());
//
// //         FatJetVector[FatJetNumber].user_info<HJetInfo>().PrintAllInfos();
//
//         Print(4, "Tag", FatJetVector[FatJetNumber].user_info<HJetInfo>().GetMaximalId(), FatJetVector[FatJetNumber].user_info<HJetInfo>().GetMaximalFraction());
//
//         JetInfo.Clear();
//
//     }
//
// //     Print(0, "");
//
// }
//
// void HDiscriminator::TagFatJets()
// {
//
//     Print(2, "Tag FatJets");
//
// //     bool HiggsLost = 1;
//
//     for (int FatJetNumber = 0; FatJetNumber < FatJetSum(); ++FatJetNumber) {
//
//         Print(2, "Fat Jet", FatJetNumber);
//         PseudoJet FatJet = FatJetVector[FatJetNumber];
//
//         vector<PseudoJet> ConstituentsVector = FatJet.constituents();
//         int ConstituentsSum = ConstituentsVector.size();
//
//         int UserIndex = EmptyId;
//
//         for (int ConstituentsNumber = 0; ConstituentsNumber < ConstituentsSum; ++ConstituentsNumber) {
//
//             int ConstUserIndex = ConstituentsVector[ConstituentsNumber].user_index();
//
//             if (ConstUserIndex != EmptyId && ConstUserIndex != EmptyUserIndex) {
//
//                 if (UserIndex == EmptyId ||
//                         (UserIndex == CpvHiggsId && ConstUserIndex != CpvHiggsId) ||
//                         (UserIndex == TopId && ConstUserIndex != TopId)) {
//
//                     Print(2, "ConstUserIndex", ConstUserIndex);
//                     UserIndex += ConstUserIndex;
//                     Print(2, "UserIndex", UserIndex);
//
//                 }
//
//             }
//
//         }
//
//         if (UserIndex > 0) {
//
//             FatJetVector[FatJetNumber].set_user_index(UserIndex);
//             Print(2, "Tagged FatJet", UserIndex);
//
//         }
//
//     }
//
// }
//
// void HDiscriminator::FatJetTagger()
// {
//
//     Print(2, "FatJetTagger");
//
//
//     if (HasHiggs) {
//
//         PseudoJet HiggsParticle = HiggsParticleVector.front();
//
//         Print(4, "Fat Jets", FatJetSum());
//         int HiggsParticlePosition = -1;
//
//         float HiggsJetParticleDistance = LargeNumber;
//         for (int FatJetNumber = 0; FatJetNumber < FatJetSum(); ++FatJetNumber) {
//
//             PseudoJet FatJet = FatJetVector[FatJetNumber];
//
//             float FatJetHiggsDistance = FatJet.delta_R(HiggsParticle);
//
//             if (FatJetHiggsDistance < HiggsJetParticleDistance) {
//
//                 HiggsJetParticleDistance = FatJetHiggsDistance;
//                 HiggsParticlePosition = FatJetNumber;
//
//             }
//
//         }
//
// //         PseudoJet Jet = FatJetVector[HiggsParticlePosition];
//
// //         if (Jet.delta_R(HiggsParticle) > (Jet.pieces()[0]).delta_R(Jet.pieces()[1]))
// //             Print(0,"Higgs too far away", Jet.delta_R(HiggsParticle) / (Jet.pieces()[0]).delta_R(Jet.pieces()[1]));
//
//         FatJetVector[HiggsParticlePosition].set_user_index(HiggsUserIndex);
//         Print(2, "Higgs Tagged", HiggsParticlePosition);
//
//
//     }
//
//     if (TopParticleSum() != 2) {
//
//         Print(1, "Number of Top Particles", TopParticleSum());
//         return ;
//
//     }
//
//     for (int TopParticleNumber = 0; TopParticleNumber < TopParticleSum(); ++TopParticleNumber) {
//
//         PseudoJet TopParticle = TopParticleVector[TopParticleNumber];
//
//         int TopParticlePostion = LargeNumber;
//
//         float TopJetParticleDistance = LargeNumber;
//         for (int FatJetNumber = 0; FatJetNumber < FatJetSum(); ++FatJetNumber) {
//
//             PseudoJet FatJet = FatJetVector[FatJetNumber];
//
//             float FatJetDistance = FatJet.delta_R(TopParticle);
//
//             if (FatJetDistance < TopJetParticleDistance) {
//
//                 TopJetParticleDistance = FatJetDistance;
//                 TopParticlePostion = FatJetNumber;
//
//             }
//
//         }
//
//         if (TopParticlePostion != LargeNumber) {
//
//             int UserIndex = FatJetVector[TopParticlePostion].user_index();
//
//             if (UserIndex > 0) {
//
//                 UserIndex += TopUserIndex;
//                 FatJetVector[TopParticlePostion].set_user_index(UserIndex);
//
//             } else {
//
//                 FatJetVector[TopParticlePostion].set_user_index(TopUserIndex);
//
//             }
//
//         }
//
//         Print(2, "Top Tagged", TopParticlePostion);
//
//     }
//
//
// }
