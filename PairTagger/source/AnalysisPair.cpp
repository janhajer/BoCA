#include "AnalysisPair.hh"
#include "Predicate.hh"
#include "Debug.hh"

std::vector<analysis::File> fusionpair::Analysis::Files(const analysis::Tag tag)
{
  Note("Set File Vector", Name(tag));

    std::vector<analysis::File> SignalLeptonicFiles;
    std::vector<analysis::File> BackgroundLeptonicFiles;
    std::vector<analysis::File> SignalSemiFiles;
    std::vector<analysis::File> BackgroundSemiFiles;


    SignalSemiFiles.emplace_back(BackgroundFile(bb,VBF));
//     SignalSemiFiles.emplace_back(analysis::File(NameString(bb), SignalCrosssection()));
//     SignalSemiFiles.emplace_back(analysis::File(SignalName(Hbb), SignalCrosssection()));

    BackgroundSemiFiles.emplace_back(BackgroundFile(bb,DYP));
    BackgroundSemiFiles.emplace_back(BackgroundFile(cc,DYP));
    BackgroundSemiFiles.emplace_back(BackgroundFile(cc,VBF));
    BackgroundSemiFiles.emplace_back(BackgroundFile(jj,DYP));
    BackgroundSemiFiles.emplace_back(BackgroundFile(jj,VBF));
//     BackgroundSemiFiles.emplace_back(BackgroundFile(ttbb));
//     BackgroundSemiFiles.emplace_back(BackgroundFile(ttcc));
//     BackgroundSemiFiles.emplace_back(BackgroundFile(ttjj));
//     BackgroundSemiFiles.emplace_back(BackgroundFile(ttqq));
//     BackgroundSemiFiles.emplace_back(BackgroundFile(ttgg));

    std::vector<analysis::File> SignalHadronicFiles;

    std::vector<analysis::File> BackgroundHadronicFiles;

    std::vector<analysis::File> LeptonicFiles = analysis::Join(SignalLeptonicFiles, BackgroundLeptonicFiles);
    std::vector<analysis::File> HadronicFiles = analysis::Join(SignalHadronicFiles, BackgroundHadronicFiles);
    std::vector<analysis::File> SemiFiles = analysis::Join(SignalSemiFiles, BackgroundSemiFiles);

    std::vector<analysis::File> NotLeptonicFiles = analysis::Join(HadronicFiles, SemiFiles);
    std::vector<analysis::File> CombinedFiles = analysis::Join(NotLeptonicFiles, LeptonicFiles);

    std::vector<analysis::File> NonLeptonicSignalFiles = analysis::Join(SignalLeptonicFiles, SignalSemiFiles);
    std::vector<analysis::File> CombinedSignalFiles = analysis::Join(SignalHadronicFiles, NonLeptonicSignalFiles);

    std::vector<analysis::File> NewFiles;

        switch (tag) {
          case analysis::Tag::signal :
            NewFiles = SignalSemiFiles;
            break;
          case analysis::Tag::background :
            NewFiles = BackgroundSemiFiles;
            break;
        }


//     NewFiles.front().SetBasePath("~/Projects/PairTagging/");
//     NewFiles.front().set_file_suffix(".root");
    SetTrees();
//     PrepareReader(Tagger, tag);
    return NewFiles;

}


void fusionpair::Analysis::SetTrees()
{

  analysis::Strings SignalLeptonicTrees {};
  analysis::Strings BackgroundLeptonicTrees {};



  analysis::Strings SignalSemiTrees {
      TreeName(bb,VBF),
//       SignalTreeName(Hbb)
    };

    analysis::Strings BackgroundSemiTrees {
        TreeName(bb,DYP),
        TreeName(cc,VBF),
        TreeName(cc,DYP),
        TreeName(jj,VBF),
        TreeName(jj,DYP),
//         TreeName(ttbb),
//         TreeName(ttcc),
//         TreeName(ttjj),
//         TreeName(ttgg),
//         TreeName(ttqq)
    };

    analysis::Strings SignalHadronicTree {};
    analysis::Strings BackgroundHadronicTrees {};

    analysis::Strings LeptonicTrees = analysis::Join(SignalLeptonicTrees, BackgroundLeptonicTrees);
    analysis::Strings HadronicTrees = analysis::Join(SignalHadronicTree, BackgroundHadronicTrees);
    analysis::Strings SemiTrees = analysis::Join(SignalSemiTrees, BackgroundSemiTrees);

    analysis::Strings NotLeptonicTrees = analysis::Join(HadronicTrees, SemiTrees);
    analysis::Strings CombinedTrees = analysis::Join(NotLeptonicTrees, LeptonicTrees);

//     switch (Tagger) {
//     case HBottomTagger:
//         BottomTagger.SetSignalTreeNames(SemiTrees);
//         BottomTagger.SetBackgroundTreeNames(SemiTrees);
//         if (Tag == Tag::signal) {
//             BottomTagger.SetTagger();
//         }
//         break;
//     case HBottomReader:
//         if (Tag == Tag::signal) {
//             BottomTagger.SetTagger();
//         }
//         BottomTagger.SetSignalTreeNames(SemiTrees);
//         BottomTagger.SetBackgroundTreeNames(SemiTrees);
//         break;
//     case JetPairTagger :
//         jet_pair_tagger.SetSignalTreeNames(SignalSemiTrees);
//         jet_pair_tagger.SetBackgroundTreeNames(BackgroundSemiTrees);
//         if (Tag == Tag::signal) {
//             jet_pair_tagger.SetTagger(BottomTagger);
//         }
//         break;
//     case HJetPairReader :
//         if (Tag == Tag::signal) {
//             jet_pair_tagger.SetTagger(BottomTagger);
//         }
//         jet_pair_tagger.SetSignalTreeNames(SignalSemiTrees);
//         jet_pair_tagger.SetBackgroundTreeNames(BackgroundSemiTrees);
//         break;
//     default :
//         Error("SetTrees", "unhandeled case");
//     }
//     tagger_.SetSignalTreeNames(SignalSemiTrees);
//     tagger_.SetBackgroundTreeNames(BackgroundSemiTrees);
}

// void hpairtagger::Analysis::PrepareReader(const analysis::Analysis::HTagger Tagger, const Tag Tag)
// {
//     Info("Prepare Reader", Tagger);
//     if (Tag == Tag::background) {
//         return;
//     }
//     switch (Tagger) {
//     case HBottomTagger:
//         break;
//     case HBottomReader:
//         BottomReader.set_tagger(bottom_tagger_);
//         break;
//     case JetPairTagger :
//         jet_pair_tagger.BottomTagger.SetTagger();
//         jet_pair_tagger.BottomReader.set_tagger(jet_pair_tagger.BottomTagger);
//         break;
//     case HJetPairReader :
//         BottomReader.set_tagger(bottom_tagger_);
//         JetPairReader.set_tagger(jet_pair_tagger);
//         break;
//     default :
//         Error("PrepareReader", "unhandled case");
//     }
// }

// void hpairtagger::Analysis::NewBranches(exroot::TreeWriter &NewTreeWriter, const analysis::Analysis::HTagger Tagger)
// {
//     Note("New Branches", Tagger);
//
//     switch (Tagger) {
//     case BottomTagger :
//         Branch = NewTreeWriter.NewBranch(StudyName(Tagger).c_str(), BottomBranch::Class());
//         break;
//     case HBottomReader :
//         Branch = NewTreeWriter.NewBranch(StudyName(Tagger).c_str(), BottomBranch::Class());
//         break;
//     case JetPairTagger :
//         Branch = NewTreeWriter.NewBranch(StudyName(Tagger).c_str(), JetPairBranch::Class());
//         break;
//     case HJetPairReader :
//         Branch = NewTreeWriter.NewBranch(StudyName(Tagger).c_str(), JetPairBranch::Class());
//         break;
//     default :
//         Error("No Branch filled");
//     }
//
// }

// int fusionpair::Analysis::RunAnalysis(analysis::Event &event, const analysis::Stage stage, const analysis::Tag tag)
// {
//     Info("Analysis", stage, tag);
//     ++event_sum_;

//     switch (stage) {
//     case BottomTagger :
//         return GetBottomTag(event, tag);
//     case HBottomReader:
//         return GetBottomReader(event, tag);
//     case JetPairTagger :
//         return GetJetPairTag(event, tag);
//     case HJetPairReader :
//         return GetJetPairReader(event, tag);
//     default :
//         Error("unknown Tagger", tagger);
//         return 0;
//     }

//     switch (stage) {
//       case analysis::Stage::trainer :
//         return tagger_.Train(event, tag);
//       case analysis::Stage::reader:
//         return tagger_.GetBdt(event, tag);
//     }
// }


// bool hpairtagger::Analysis::GetTag(analysis::Event &event, const Tag tag)
// {
//   Debug("Bottom Tag", Name(tag));
//   return tagger_.GetBranches(event, tag);
// }
//
//
// bool hpairtagger::Analysis::GetBottomTag(analysis::Event &event, const Tag Tag)
// {
//     Debug("Bottom Tag", Tag);
//     std::vector<BottomBranch> Bottoms = BottomTagger.GetBranches(event, Tag);
//     if (Bottoms.empty()) {
//         return 0;
//     }
//     for (const auto & Bottom : Bottoms) {
//         *static_cast<BottomBranch *>(Branch->NewEntry()) = Bottom;
//     }
//     return 1;
// }
//
// bool hpairtagger::Analysis::GetBottomReader(analysis::Event &event, const Tag Tag)
// {
//     Debug("Bottom Reader", Tag);
//     Jets jets = bottom_tagger_.GetJets(event);
//     jets = bottom_tagger_.GetJetBdt(jets, BottomReader);
//     if (jets.empty()) {
//         return 0;
//     }
//
//     Jets Particles = event.Partons().GenParticles();
//     Particles.erase(std::remove_if(Particles.begin(), Particles.end(), WrongAbsId(Id::bottom)), Particles.end());
//
//     for (const auto & Particle : Particles) {
//         std::sort(jets.begin(), jets.end(), MinDeltaR(Particle));
//         if (jets.front().delta_R(Particle) < BottomTagger.detector_geometry().JetConeSize()) {
//             static_cast<analysis::JetInfo *>(jets.front().user_info_shared_ptr().get())->SetTag(Signal);
//         }
//     }
//
//     for (const auto & Jet : jets)  {
//         if (Tag != Jet.user_info<analysis::JetInfo>().Tag()) {
//             continue;
//         }
// //         if (std::abs(Jet.rap()) > BottomTagger.detector_geometry().TrackerEtaMax) {
// //             continue;
// //         }
//         *static_cast<BottomBranch *>(Branch->NewEntry()) = BottomTagger.GetBranch(Jet);
//     }
//     return 1;
// }
//
// bool hpairtagger::Analysis::GetJetPairTag(analysis::Event &event, const Tag Tag)
// {
//     Debug("JetPair Tag", Tag);
//     std::vector<JetPairBranch> JetPairs = jet_pair_tagger.GetBranches(event, Tag, MotherId(ProductionChannel()));
//     if (JetPairs.empty()) {
//         return 0;
//     }
//     for (const auto & JetPair : JetPairs) {
//         *static_cast<JetPairBranch *>(Branch->NewEntry()) = JetPair;
//     }
//     return 1;
// }
//
// bool hpairtagger::Analysis::GetJetPairReader(analysis::Event &event, const Tag Tag)
// {
//     Debug("JetPair Reader", Tag);
//     Jets jets = bottom_tagger_.GetJets(event);
//     jets = bottom_tagger_.GetJetBdt(jets, BottomReader);
//     if (jets.empty()) {
//         Info("just one jet", jets.size());
//         return 0;
//     }
//     Error("jets", jets.size());
//
// //     Jets FilteredJets; // WRONG MUST BE REMOVED
//     Jets Particles = event.Partons().GenParticles();
//     if (Tag == Tag::signal) {
// //         Particles = BottomTagger.RemoveIfWrongAbsFamily(Particles, Id::bottom, MotherId(ProductionChannel()));
//     }
//     if (
// // ProductionChannel() == Associated &&
//         Tag == Tag::background) {
// //         Particles = RemoveIfWrongAbsStepMother(Particles, Id::top); // THIS IS WRONG AND SHOULD BE REMOVED AGAIN
// //         Particles = BottomTagger.RemoveIfWrongParticle(Particles, Id::gluon); // THIS IS WRONG AND SHOULD BE REMOVED AGAIN
// //         Particles = BottomTagger.RemoveIfWrongAbsMother(Particles, Id::Z); // THIS IS WRONG AND SHOULD BE REMOVED AGAIN
//     }
//
// //     if (
// //         Tag == Tag::signal &&
// //         Particles.size() != 2) {
// //         Error("where are the quarks?", Particles.size());
// //     }
//
// //     for (const auto & Particle : Particles) {
// //         std::sort(jets.begin(), jets.end(), MinDeltaR(Particle));
// //         if (jets.front().delta_R(Particle) < BottomTagger.detector_geometry().JetConeSize()) {
// //             static_cast<analysis::JetInfo *>(jets.front().user_info_shared_ptr().get())->SetTag(Signal);
// //             FilteredJets.emplace_back(jets.front()); // WRONG MUST BE REMOVED
// //         }
// //     }
//
// //     std::vector<analysis::Doublet> doublets = jet_pair_tagger.GetBdt(FilteredJets, JetPairReader);
//     std::vector<analysis::Doublet> doublets = jet_pair_tagger.GetBdt(jets, JetPairReader);
//     if (doublets.size() > 1) {
//         std::sort(doublets.begin(), doublets.end());
//         doublets.erase(doublets.begin() + 1, doublets.end());
//     }
//
//     for (const auto & doublet : doublets) {
//         static_cast<JetPairBranch &>(*Branch->NewEntry()) = jet_pair_tagger.GetBranch(doublet);
//     }
//     return 1;
// }