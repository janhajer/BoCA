#include "AnalysisZTagger.hh"
#include "Debug.hh"

namespace analysis
{

  namespace standardmodel
{

AnalysisZ::AnalysisZ(Tagger &tagger) : analysis::standardmodel::Analysis::Analysis(tagger)
{
    Note();
    this->tagger().set_analysis_name(ProjectName());
    pre_cuts().SetPtLowerCut(Id::Z, LowerPtCut());
    pre_cuts().SetPtUpperCut(Id::Z, UpperPtCut());
    pre_cuts().SetMassUpperCut(Id::Z, 200);
    //     pre_cuts().SetTrackerMaxEta(Id::Z, DetectorGeometry().TrackerEtaMax);
}

void AnalysisZ::SetFiles(const Tag tag)
{
    Note("Set File Vector", Name(tag));
    switch (tag) {
    case Tag::signal :
        NewFile(tag,Process::zz);
        break;
    case Tag::background :
      NewFile(tag,Process::tt_had);
      NewFile(tag,Process::hh);
      NewFile(tag,Process::ww);
      NewFile(tag,Process::bb);
      NewFile(tag,Process::cc);
      NewFile(tag,Process::qq);
      NewFile(tag,Process::gg);
        break;
    }

}

int AnalysisZ::PassPreCut(const Event &event)
{
    Info("pass pre cut");
    Jets leptons = fastjet::sorted_by_pt(event.Leptons().leptons());
    if(leptons.empty()) return 1;
    if(leptons.front().pt()<80) return 1;
   return 0;
}
std::string AnalysisZ::ProjectName() const
{
    return  "ZTagger-" + ColliderName(collider_type()) + "-" + std::to_string(PreCut()) + "GeV-test";
}

}

}
