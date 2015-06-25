# include "AnalysisHiggs.hh"

namespace analysis
{

namespace higgscpv
{

Analysis::Analysis(Tagger &tagger) : analysis::Analysis::Analysis(tagger)
{
    Print(Severity::Notification, "Constructor");
    this->tagger().set_analysis_name(ProjectName());
}

std::string Analysis::ProcessName()
{
    return "higgscpv";
}

void Analysis::SetFiles(const Tag tag)
{
    Print(Severity::Notification, "Set Files");
    switch (tag) {
    case Tag::Signal :
        NewSignalFile("pp-ttx0-bbbbllnunu-1", 0.02071);
//         NewSignalFile("pp-ttx0-bbbbllnunu-0", 0.008937);
//         NewSignalFile("pp-ttx0-bbbbllnunu-0.5", 0.01193);
        break;
    case Tag::Background :
        NewBackgroundFile("pp-ttbb-bbbbllnunu", 3.457);
        break;
    }
}

int Analysis::PassPreCut(Event &event)
{
    Print(Severity::Information, "pass pre cut");
//   Jets particles = event.Partons().GenParticles();
//   Jets tops = fastjet::sorted_by_pt(copy_if_abs_particle(particles, Id::Top));
//   remove_if_not_in_pt_window(tops, PreCut(), UpperCut());
    return 1;
}

}

}
