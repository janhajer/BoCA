#include "AnalysisTopPartner.hh"
#include "Factory.hh"
#include <sys/stat.h>

namespace analysis
{

namespace toppartner
{

Analysis::Analysis(Tagger &tagger) : analysis::Analysis::Analysis(tagger)
{
    Print(Severity::notification, "Constructor");
    this->tagger().set_analysis_name(ProjectName());
}

std::string Analysis::ProcessName()
{
    return "toppartner";
}

void Analysis::SetFiles(const Tag tag)
{
    Print(Severity::notification, "Set Files");
    switch (tag) {
    case Tag::signal :
//         NewSignalFile("pp-Tth-bbbbjjjjlv");
        NewSignalFile("pp-TThh-bbbbbbjjlv");
//         if(tagger().name() == "Bottom") NewSignalFile("pp-ttbbj-bbbbjjlv");
        break;
    case Tag::background :
//         NewBackgroundFile("pp-ttbb-bbbbjjlv");
        NewBackgroundFile("pp-tthjj-bbbbjjjjlv");
        break;
    }
}

int Analysis::PassPreCut(Event &event)
{
    Print(Severity::information, "pass pre cut");
    return 1;
}

void Analysis::RunFast()
{
    RunTagger(analysis::Stage::trainer);
    RunFactory();
}

void Analysis::RunNormal()
{
    RunFast();
    RunTagger(analysis::Stage::reader);
}

void Analysis::RunFull()
{
    RunNormal();
    RunReader();
}

void Analysis::RunFullTagger()
{
    RunNormal();
    RunReaderTagger();
}

void Analysis::RunTagger(Stage stage)
{
    if (Missing(PathName(tagger().name(stage)))) AnalysisLoop(stage);
}

void Analysis::RunFactory()
{
    PrepareFiles();
    if (Missing(PathName(tagger().factory_name()))) analysis::Factory factory(tagger());
}

void Analysis::RunReader()
{
    PrepareFiles();
    if (Missing(PathName(tagger().bdt_weight_name()))) {
        analysis::Reader reader(tagger());
        reader.OptimalSignificance();
    }
}

void Analysis::RunReaderTagger()
{
    PrepareFiles();
    if (Missing(PathName(tagger().bdt_weight_name()))) {
        analysis::Reader reader(tagger());
        reader.TaggingEfficiency();
    }
}

std::string Analysis::PathName(const std::string &file_name) const
{
    Print(Severity::error, "Path Name", file_name);
    return ProjectName() + "/" + file_name + ".root";
}

}

}
