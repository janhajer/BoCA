#include "HAnalysisTau.hh"

#include "fastjet/LimitedWarning.hh"

void RunTagger(const hanalysis::HAnalysis::HTagger Tagger)
{
    hheavyhiggs::HAnalysisTau Analysis;
    const std::string Name = Analysis.StudyName(Tagger);
    Analysis.Print(Analysis.HError, "Tagger", Tagger, Name);

    std::string FileName = Analysis.ProjectName() + "/" + Name + ".root";
    if (gSystem->AccessPathName(FileName.c_str())) Analysis.AnalysisLoop(Tagger);

    FileName = Analysis.ProjectName() + "/Mva" + Name + ".root";
    if (gSystem->AccessPathName(FileName.c_str())) {
        switch (Tagger) {
        case hanalysis::HAnalysis::HTauTagger:
            hanalysis::HFactory(Analysis.TauTagger);
            break;
//         case hanalysis::HAnalysis::HJetPairTagger:
//             hanalysis::HFactory(Analysis.JetPairTagger);
//             break;
//         case hanalysis::HAnalysis::HWHadronicTagger:
//             hanalysis::HFactory(Analysis.WHadronicTagger);
//             break;
        case hanalysis::HAnalysis::HHeavyHiggsTauTagger:
            hanalysis::HFactory(Analysis.HiggsTauTagger);
            break;
//         case hanalysis::HAnalysis::HTopLeptonicTagger:
//             hanalysis::HFactory(Analysis.TopLeptonicTagger);
//             break;
//         case hanalysis::HAnalysis::HTopHadronicTagger:
//             hanalysis::HFactory(Analysis.TopHadronicTagger);
//             break;
//         case hanalysis::HAnalysis::HTopSemiTagger:
//             hanalysis::HFactory(Analysis.TopSemiTagger);
//             break;
//         case hanalysis::HAnalysis::HHeavyHiggsLeptonicTagger:
// //             hanalysis::HFactory(Analysis.HeavyHiggsLeptonicTagger);
//             break;
//         case hanalysis::HAnalysis::HHeavyHiggsHadronicTagger:
// //             hanalysis::HFactory(Analysis.HeavyHiggsHadronicTagger);
//             break;
//         case hanalysis::HAnalysis::HHeavyHiggsSemiTagger:
//             hanalysis::HFactory(Analysis.ChargedHiggsSemiTagger);
//             break;
//         case hanalysis::HAnalysis::HEventLeptonicTagger:
// //             hanalysis::HFactory(Analysis.EventLeptonicTagger);
//             break;
//         case hanalysis::HAnalysis::HEventHadronicTagger:
// //             hanalysis::HFactory(Analysis.EventHadronicTagger);
//             break;
//         case hanalysis::HAnalysis::HEventSemiTagger:
//             hanalysis::HFactory(Analysis.EventSemiTagger);
//             break;
//         case hanalysis::HAnalysis::HChargedHiggsSemiTagger:
//           hanalysis::HFactory(Analysis.ChargedHiggsSemiTagger);
//           break;
        default:
            std::cout << "Unhandled case" << std::endl;
        }
    }

    FileName = Analysis.ProjectName() + "/" + Name + "Bdt.root";
    if (gSystem->AccessPathName(FileName.c_str())) {
        switch (Tagger) {
        case hanalysis::HAnalysis::HEventLeptonicReader: {
//             hanalysis::HReader Reader(Analysis.EventLeptonicTagger);
//             Reader.SimpleMVALoop();
            break;
        }
        case hanalysis::HAnalysis::HEventSemiReader: {
//           Analysis.SetTrees(Tagger,hanalysis::HAnalysis::HSignal);
//             hanalysis::HReader Reader(Analysis.EventSemiTagger);
//             Reader.SimpleMVALoop();
            break;
        }
        default:
            std::cout << "Unhandled case" << std::endl;
        }
    }
}

int main()
{
    fastjet::Error::set_print_errors(true);
    fastjet::Error::set_print_backtrace(true);

    RunTagger(hanalysis::HAnalysis::HTauTagger);
    RunTagger(hanalysis::HAnalysis::HTauReader);

//     RunTagger(hanalysis::HAnalysis::HWHadronicTagger);
//     RunTagger(hanalysis::HAnalysis::HWHadronicReader);

    RunTagger(hanalysis::HAnalysis::HHeavyHiggsTauTagger);
    RunTagger(hanalysis::HAnalysis::HHeavyHiggsTauReader);

//     RunTagger(hanalysis::HAnalysis::HTopLeptonicTagger);
//     RunTagger(hanalysis::HAnalysis::HTopLeptonicReader);

//     RunTagger(hanalysis::HAnalysis::HTopHadronicTagger);
//     RunTagger(hanalysis::HAnalysis::HTopHadronicReader);

//     RunTagger(hanalysis::HAnalysis::HTopSemiTagger);
//     RunTagger(hanalysis::HAnalysis::HTopSemiReader);

//     RunTagger(hanalysis::HAnalysis::HHeavyHiggsLeptonicTagger);
//     RunTagger(hanalysis::HAnalysis::HHeavyHiggsLeptonicReader);

//     RunTagger(hanalysis::HAnalysis::HEventLeptonicTagger);
//     RunTagger(hanalysis::HAnalysis::HEventLeptonicReader);
//
//     RunTagger(hanalysis::HAnalysis::HHeavyHiggsSemiTagger);
//     RunTagger(hanalysis::HAnalysis::HHeavyHiggsSemiReader);

//     RunTagger(hanalysis::HAnalysis::HJetPairTagger);
//     RunTagger(hanalysis::HAnalysis::HJetPairReader);


//     RunTagger(hanalysis::HAnalysis::HEventSemiTagger);
//     RunTagger(hanalysis::HAnalysis::HEventSemiReader);

//     RunTagger(hanalysis::HAnalysis::HHeavyHiggsHadronicTagger);
//     RunTagger(hanalysis::HAnalysis::HEventHadronicTagger);

//     RunTagger(hanalysis::HAnalysis::HChargedHiggsSemiTagger);

    std::cout << fastjet::LimitedWarning::summary() << std::endl;



    return 0;

}
