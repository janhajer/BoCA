#include "HAnalysisHeavyHiggsTt.hh"

#include "fastjet/LimitedWarning.hh"

void RunTagger(hanalysis::Tagger &tagger, hanalysis::Tagger::Stage stage)
{
    hheavyhiggs::HAnalysisTt Analysis(tagger);
    const std::string Name = tagger.tagger_name();
    Analysis.Print(Analysis.kError, "Tagger", Name);

    std::string FileName = Analysis.ProjectName() + "/" + Name + ".root";
    if (gSystem->AccessPathName(FileName.c_str())) Analysis.AnalysisLoop(stage);

    FileName = Analysis.ProjectName() + "/Mva" + Name + ".root";
    if (gSystem->AccessPathName(FileName.c_str())) {
//         switch (Tagger) {
//         case hanalysis::HAnalysis::HBottomTagger:
            hanalysis::Factory factory(tagger);
//             break;
//         case hanalysis::HAnalysis::HWHadronicTagger:
//             hanalysis::Factory(Analysis.WHadronicTagger);
//             break;
//         case hanalysis::HAnalysis::HWSemiTagger:
//             hanalysis::Factory(Analysis.WSemiTagger);
//             break;
//         case hanalysis::HAnalysis::HTopHadronicTagger:
//             hanalysis::Factory(Analysis.TopHadronicTagger);
//             break;
//         case hanalysis::HAnalysis::HTopSemiTagger:
//             hanalysis::Factory(Analysis.TopSemiTagger);
//             break;
//         case hanalysis::HAnalysis::HHeavyHiggsSemiTagger:
//             hanalysis::Factory(Analysis.HeavyHiggsSemiTagger);
//             break;
//         case hanalysis::HAnalysis::HEventSemiTagger:
//             hanalysis::Factory(Analysis.EventSemiTagger);
//             break;
//         default:
//             std::cout << "Unhandled case" << std::endl;
//         }
    }

    FileName = Analysis.ProjectName() + "/" + Name + "Bdt.root";
    if (gSystem->AccessPathName(FileName.c_str())) {
//         switch (Tagger) {
//         case hanalysis::HAnalysis::HEventSemiReader: {
            hanalysis::Reader Reader(tagger);
            Reader.OptimalSignificance();
//             break;
//         }
//         default:
//             std::cout << "Unhandled case" << std::endl;
//         }
    }
}

int main()
{
    fastjet::Error::set_print_errors(true);
    fastjet::Error::set_print_backtrace(true);

    hanalysis::BottomTagger bottom_tagger;
    RunTagger(bottom_tagger, hanalysis::Tagger::kTrainer);
    RunTagger(bottom_tagger, hanalysis::Tagger::kReader);

    hanalysis::HWHadronicTagger w_hadronic_tagger;
    RunTagger(w_hadronic_tagger, hanalysis::Tagger::kTrainer);
    RunTagger(w_hadronic_tagger, hanalysis::Tagger::kReader);

    hanalysis::HWSemiTagger w_semi_tagger;
    RunTagger(w_semi_tagger, hanalysis::Tagger::kTrainer);
    RunTagger(w_semi_tagger, hanalysis::Tagger::kReader);

    hanalysis::HTopHadronicTagger top_hadronic_tagger;
    RunTagger(top_hadronic_tagger, hanalysis::Tagger::kTrainer);
    RunTagger(top_hadronic_tagger, hanalysis::Tagger::kReader);

    hanalysis::HTopSemiTagger tops_semi_tagger;
    RunTagger(tops_semi_tagger, hanalysis::Tagger::kTrainer);
    RunTagger(tops_semi_tagger, hanalysis::Tagger::kReader);

    hanalysis::HHeavyHiggsSemiTagger heavy_higgs_semi_tagger;
    RunTagger(heavy_higgs_semi_tagger, hanalysis::Tagger::kTrainer);
    RunTagger(heavy_higgs_semi_tagger, hanalysis::Tagger::kReader);


    hheavyhiggs::HEventTtSemiTagger event_semi_tagger;
    RunTagger(event_semi_tagger, hanalysis::Tagger::kTrainer);
    RunTagger(event_semi_tagger, hanalysis::Tagger::kReader);


    std::cout << fastjet::LimitedWarning::summary() << std::endl;



    return 0;

}

