# include "HBranchTopTagger.hh"

ClassImp(htoptagger::HCandidateBranch)

htoptagger::HCandidateBranch::HCandidateBranch()
{

    reset();

}

htoptagger::HCandidateBranch::~HCandidateBranch() {}

void htoptagger::HCandidateBranch::reset()
{

    int InitialValue = -10;

    Mass = InitialValue;
    Pt = InitialValue;
    Rap = InitialValue;
    Phi = InitialValue;

    DeltaR = InitialValue;
    SubJetsDeltaR = InitialValue;
    Asymmetry = InitialValue;
    DiPolarity = InitialValue;

    SubJet1Mass = InitialValue;
    SubJet1Pt = InitialValue;
    SubJet1DeltaR = InitialValue;

    SubJet2Mass = InitialValue;
    SubJet2Pt = InitialValue;
    SubJet2DeltaR = InitialValue;

    ConstRap = InitialValue;
    ConstPhi = InitialValue;
    ConstDeltaR = InitialValue;
    ConstAngle = InitialValue;

    IsolationRap = InitialValue;
    IsolationPhi = InitialValue;
    IsolationDeltaR = InitialValue;
    IsolationAngle = InitialValue;
    IsolationPt = InitialValue;

    HiggsTag = InitialValue;
    TopTag = InitialValue;

    TopBdtTag = InitialValue;
    JetBdtTag = InitialValue;

    TopCutSigEff = InitialValue;
    JetCutSigEff = InitialValue;

}

// ClassImp(HLeptonBranch)
//
// HLeptonBranch::HLeptonBranch()
// {
//
//     reset();
//
// }
//
// HLeptonBranch::~HLeptonBranch() {}
//
// void HLeptonBranch::reset()
// {
//
//     int InitialValue = -10;
//
//     Mass = InitialValue;
//     Pt = InitialValue;
//     Rap = InitialValue;
//     Phi = InitialValue;
//     Charge = InitialValue;
//
// }
//
//
// ClassImp(HConstituentBranch)
//
// HConstituentBranch::HConstituentBranch()
// {
//
//   reset();
//
// }
//
// HConstituentBranch::~HConstituentBranch() {}
//
// void HConstituentBranch::reset()
// {
//
//   int InitialValue = -100;
//
//   Rap = InitialValue;
//   Phi = InitialValue;
//   Pt = InitialValue;
//   Id = InitialValue;
//
// }
//
// ClassImp(HJetPropertiesBranch)
//
// HJetPropertiesBranch::HJetPropertiesBranch()
// {
//
//     reset();
//
// }
//
// HJetPropertiesBranch::~HJetPropertiesBranch() {}
//
// void HJetPropertiesBranch::reset()
// {
//
//     int InitialValue = -10;
//
//     Mass = InitialValue;
//     Pt = InitialValue;
//
//     Rap = InitialValue;
//     Phi = InitialValue;
//     Radius = InitialValue;
//
// }
//
//
// ClassImp(HConstituentBranch)
