#include "BranchesHiggsCpv.hh"
#include "Pair.hh"

namespace analysis
{
namespace higgscpv
{

  SignatureTTaggerBranch::SignatureTTaggerBranch()
  {
    Mass12 = InitialValue();
    Mass23 = InitialValue();
    Mass13 = InitialValue();
    DeltaPt23 = InitialValue();
    DeltaPt13 = InitialValue();
    Ht12 = InitialValue();
    Ht23 = InitialValue();
    Ht13 = InitialValue();
    DeltaRap23 = InitialValue();
    DeltaRap13 = InitialValue();
    DeltaPhi23 = InitialValue();
    DeltaPhi13 = InitialValue();
    DeltaR23 = InitialValue();
    DeltaR13 = InitialValue();
    DeltaM23 = InitialValue();
    DeltaM13 = InitialValue();
    DeltaHt23 = InitialValue();
    DeltaHt13 = InitialValue();
    Sphericity = InitialValue();
    Aplanarity = InitialValue();
  }
  Observables SignatureTTaggerBranch::Variables() const
  {
    return Join(MultiBranch::Variables(), {PAIR(Aplanarity), PAIR(Sphericity)});
  }

SignatureLeptonicBranch::SignatureLeptonicBranch()
{
    HiggsMass = InitialValue();
    PairRap = InitialValue();
    BottomBdt = InitialValue();
    PairBottomBdt = InitialValue();
    HardTopPt = InitialValue();
    SoftTopPt = InitialValue();
    Aplanarity = InitialValue();
    Sphericity = InitialValue();
}

OctetBranch::OctetBranch()
{
    HiggsMass = InitialValue();
    PairRap = InitialValue();
    BottomBdt = InitialValue();
    PairBottomBdt = InitialValue();
    HiggsBdt = InitialValue();
    PairBdt = InitialValue();
    HardTopPt = InitialValue();
    SoftTopPt = InitialValue();
}

EventBranch::EventBranch() {}

TopLeptonicPairBranch::TopLeptonicPairBranch() {}

TripletPairBranch::TripletPairBranch()
{
    BottomMass = InitialValue();
    BottomPt = InitialValue();
    BottomRap = InitialValue();
    BottomPhi = InitialValue();
    BottomBdt = InitialValue();
    BottomBTag = InitialValue();
    TopMass = InitialValue();
    TopPt = InitialValue();
    TopRap = InitialValue();
    TopPhi = InitialValue();
    TopBdt = InitialValue();
    TopBTag = InitialValue();
}

QuartetPairBranch::QuartetPairBranch()
{
    BottomMass = InitialValue();
    BottomPt = InitialValue();
    BottomRap = InitialValue();
    BottomPhi = InitialValue();
    BottomBdt = InitialValue();
    BottomBTag = InitialValue();
    TopMass = InitialValue();
    TopPt = InitialValue();
    TopRap = InitialValue();
    TopPhi = InitialValue();
    TopBdt = InitialValue();
    TopBTag = InitialValue();
}
Observables SignatureLeptonicBranch::Variables() const
{
    return Join(MultiBranch::Variables(), {PAIR(BottomBdt), PAIR(PairBottomBdt), PAIR(HardTopPt), PAIR(SoftTopPt), PAIR(HiggsMass), PAIR(PairRap), PAIR(Aplanarity), PAIR(Sphericity)});
}
Observables TripletPairBranch::Variables() const
{
    return Join(PairBranch::Variables(), {PAIR(BottomPt), PAIR(BottomRap), PAIR(BottomPhi), PAIR(BottomMass), PAIR(TopPt), PAIR(TopRap), PAIR(TopPhi), PAIR(TopMass), PAIR(TopBdt)});
}
Observables EventBranch::Variables() const
{
  return Join(MultiBranch::Variables(), {PAIR(JetNumber), PAIR(BottomNumber), PAIR(MissingEt), PAIR(ScalarHt), PAIR(LeptonHt), PAIR(JetMass), PAIR(JetPt), PAIR(JetHt), PAIR(JetRap), PAIR(JetPhi)});
}
Observables EventBranch::Spectators() const
{
    return Join(MultiBranch::Variables(), {PAIR(LeptonNumber)});
}
}
}

