# include "HBranchTest.hh"

ClassImp(htest::HCandidateBranch)

htest::HCandidateBranch::HCandidateBranch()
{

    reset();

}

htest::HCandidateBranch::~HCandidateBranch() {}

void htest::HCandidateBranch::reset()
{

    int InitialValue = -10;

    Mass = InitialValue;
    Pt = InitialValue;
    Rap = InitialValue;
    Phi = InitialValue;
    Area = InitialValue;

    SubJetsDeltaR = InitialValue;

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
