# include "AnalysisHiggs.hh"

higgscpv::Analysis::Analysis(analysis::Tagger &tagger) : analysis::Analysis::Analysis(tagger){
  Print(kNotification, "Constructor");
  tagger_.set_analysis_name(ProjectName());
}

std::string higgscpv::Analysis::ProcessName(){
  return "higgscpv";
}

void higgscpv::Analysis::SetFiles(const Tag tag)
{
    Print(kNotification, "Set Files");

    NewBackgroundFile("pp-bbtt-bblvlv");
//       analysis::File *Background = new analysis::File("pp-bbtt-bblvlv", "background");
//         Background->set_crosssection(3.215); // pb
//         Background->set_crosssection_error(0.012); // pb
//         files.emplace_back(Background);

//     }

    NewSignalFile("pp-x0tt-bblvlv-even");
//     analysis::File *Even = new analysis::File("pp-x0tt-bblvlv", "even");
//     Even->set_crosssection(30.02079); // pb
//     Even->set_crosssection_error(0.000078); // pb
//     Even->TagString="tag_2";
//     files.emplace_back(Even);

    NewSignalFile("pp-x0tt-bblvlv-mix");
//     analysis::File *Mix = new analysis::File("pp-x0tt-bblvlv", "mix");
//     Mix->set_crosssection(30.01172); // pb
//     Mix->set_crosssection_error(0.000045); // pb
//     Mix->TagString="tag_2";
//     files.emplace_back(Mix);

    NewSignalFile("pp-x0tt-bblvlv-odd");
//     analysis::File *Odd = new analysis::File("pp-x0tt-bblvlv", "odd");
//     Odd->set_crosssection(30.008951); // pb
//     Odd->set_crosssection_error(0.000035); // pb
//     Odd->TagString="tag_2";
//     files.emplace_back(Odd);

//     hdelphes::HFile *Test = new hdelphes::HFile("pp-hjj-bbjj");
//     Files.emplace_back(Test);
//     hdelphes::HFile *Signal = new hdelphes::HFile("pp-htt-bblvlv", "signal");
//     Signal->Crosssection = 0.01419; // pb
//     Signal->Error = 0.000067; // pb
// //     Odd->TagString="tag_2";
//     Files.emplace_back(Signal);

//     Print(kNotification, "Files prepared");




//     bottom_tagger_ = analysis::BottomTagger();
//     bottom_tagger_.set_analysis_name(ProjectName());
//     BottomTagger.SetTestTreeNames( {"pp-bbtt-bblvlv-background", "pp-x0tt-bblvlv-even", "pp-x0tt-bblvlv-mix", "pp-x0tt-bblvlv-odd"});
//     BottomTagger.SetSignalTreeNames( {"pp-bbtt-bblvlv-background", "pp-x0tt-bblvlv-even", "pp-x0tt-bblvlv-mix", "pp-x0tt-bblvlv-odd"});
//     BottomTagger.SetBackgroundTreeNames( {"pp-bbtt-bblvlv-background", "pp-x0tt-bblvlv-even", "pp-x0tt-bblvlv-mix", "pp-x0tt-bblvlv-odd"});

//     if (NewStudyName != "Bottom" && NewStudyName != "NotBottom") {

//       BottomReader.set_tagger(bottom_tagger_);

//       LeptonicTopTagger = analysis::TopLeptonicTagger();
//         LeptonicTopTagger.set_analysis_name(ProjectName());
//         LeptonicTopTagger.SetTestTreeNames( {"pp-bbtt-bblvlv-background", "pp-x0tt-bblvlv-even", "pp-x0tt-bblvlv-mix", "pp-x0tt-bblvlv-odd"});
//         LeptonicTopTagger.SetSignalTreeNames( {"pp-bbtt-bblvlv-background", "pp-x0tt-bblvlv-even", "pp-x0tt-bblvlv-mix", "pp-x0tt-bblvlv-odd"});
//         LeptonicTopTagger.SetBackgroundTreeNames( {"pp-bbtt-bblvlv-background", "pp-x0tt-bblvlv-even", "pp-x0tt-bblvlv-mix", "pp-x0tt-bblvlv-odd"});



//         HiggsTagger = analysis::HMvaHiggsTagger(bottom_tagger_);
//         HiggsTagger.set_analysis_name(ProjectName());
//         HiggsTagger.SetTestTreeNames( {"pp-bbtt-bblvlv-background", "pp-x0tt-bblvlv-even", "pp-x0tt-bblvlv-mix", "pp-x0tt-bblvlv-odd"});
//         HiggsTagger.SetSignalTreeNames( {"pp-x0tt-bblvlv-even", "pp-x0tt-bblvlv-mix", "pp-x0tt-bblvlv-odd"});
//         HiggsTagger.SetBackgroundTreeNames( {"pp-bbtt-bblvlv-background", "pp-x0tt-bblvlv-even", "pp-x0tt-bblvlv-mix", "pp-x0tt-bblvlv-odd"});
//
//         if (NewStudyName != "Top" && NewStudyName != "NotTop" && NewStudyName != "Higgs" && NewStudyName != "NotHiggs") {
//           HiggsReader.set_tagger(HiggsTagger);
//           TopReader.set_tagger(LeptonicTopTagger);
//         }

//     }

//     return files;

}


// void higgscpv::HAnalysis::NewBranches(exroot::TreeWriter *NewTreeWriter, const HTagger Tagger)
// {
//     Print(kNotification, "New Branches", Tagger);
//
//     if (Tagger == HBottomTagger) {
//         bottom_branch = NewTreeWriter->NewBranch("Bottom", BottomBranch::Class());
//     } else if (Tagger == TopLeptonicTagger) {
//         TopBranch = NewTreeWriter->NewBranch("Top", TopLeptonicBranch::Class());
//     } else if (Tagger == HHiggsLeptonicTagger) {
//         HiggsBranch = NewTreeWriter->NewBranch("Higgs", HiggsBranch::Class());
//         constituentBranch = NewTreeWriter->NewBranch("constituent", ParticleBranch::Class());
//     } else if (Tagger == EventTagger) {
//         eventBranch = NewTreeWriter->NewBranch("event", EventBranch::Class());
//     }
// //     } else {
// //         constituentBranch = TreeWriter->NewBranch("constituent", ParticleBranch::Class());
// }


int higgscpv::Analysis::PassPreCut(analysis::Event &event)
{
  Print(kInformation, "pass pre cut");
  analysis::Jets particles = event.Partons().GenParticles();
  analysis::Jets tops = fastjet::sorted_by_pt(copy_if_abs_particle(particles, TopId));
//   analysis::remove_if_not_in_pt_window(tops, PreCut(), UpperCut());
  return tops.size();
}

int higgscpv::Analysis::RunAnalysis(analysis::Event &event, const analysis::Tagger::Stage stage, const analysis::Object::Tag tag)
{
  Print(kInformation, "Analysis");
  switch (stage) {
    case analysis::Tagger::kTrainer :
      return tagger_.Train(event, pre_cuts_, tag);
    case analysis::Tagger::kReader :
      return reader_.GetBdt(event, pre_cuts_);
    default : return 0;
  }
}

// bool higgscpv::Analysis::GetBottomTag(analysis::Event &, const std::string &NewStudyName)
// {
//
//     Print(kDebug, "Bottom Tag", NewStudyName);

//     HTag State;
//     if (StudyName == "Bottom") State = kSignal;
//     if (StudyName == "NotBottom") State = HBackground;

//     std::vector<BottomBranch *> Bottoms = BottomTagger.GetBranches(event, State);
//
//     for (const auto & Bottom : Bottoms) {
//         BottomBranch *BTagger = static_cast<BottomBranch *>(bottom_branch->NewEntry());
//         *BTagger = *Bottom;
//     }

//     return 1;

// }


// bool higgscpv::Analysis::GetTopTag(analysis::Event &event, const std::string &NewStudyName)
// {
//
//     Print(kInformation, "Tops", NewStudyName);
//
//     Tag State;
//     if (NewStudyName == "Top") State = kSignal;
//     if (NewStudyName == "NotTop") State = kBackground;

//     return LeptonicTopTagger.Train(event, State);

//     for (const auto & Top : Tops) *static_cast<analysis::TopLeptonicBranch *>(TopBranch->NewEntry()) = Top;

//     return 1;

// }


// // bool higgscpv::Analysis::GetHiggsTag(analysis::Event &event, const std::string &NewStudyName)
// {
//     Print(kInformation, "Higgs Tag", NewStudyName);
//
//     Tag State;
//     if (NewStudyName == "Higgs") State = kSignal;
//     if (NewStudyName == "NotHiggs") State = kBackground;
//
//     std::vector<analysis::HiggsBranch *> Higgses ;//= HiggsTagger.GetBranches(event, State);
//
//     for (const auto & Higgs : Higgses) {
//       analysis::HiggsBranch *HiggsBranch1 = static_cast<analysis::HiggsBranch *>(HiggsBranch->NewEntry());
//         *HiggsBranch1 = *Higgs;
//     }
//
//     std::vector<analysis::ParticleBranch *> Constitents;// = HiggsTagger.GetconstituentBranches();
//
//     for (const auto & constituent : Constitents) {
//       analysis::ParticleBranch *constituentBranch1 = static_cast<analysis::ParticleBranch *>(constituentBranch->NewEntry());
//         *constituentBranch1 = *constituent;
//     }
//
//     return 1;
//
// }
//
//
// struct SortHiggsCpv {
//     inline bool operator()(const higgscpv::HHiggsCpv &event1, const higgscpv::HHiggsCpv &event2) {
//         return (event1.Bdt() > event2.Bdt());
//     }
// };

// bool higgscpv::Analysis::GetSignalTag(analysis::Event &event, const std::string &NewStudyName)
// {
//     Print(kInformation, "event", NewStudyName);
//
//     analysis::Jets Leptons = event.Leptons().leptons();
//     if (Leptons.size() < 2) {
//         Print(kNotification, "Not enough Leptons", Leptons.size());
//         return 0;
//     }
//
//     analysis::Jets jets = event.Hadrons().Jets();
//     if (jets.size() < 4) {
//         Print(kNotification, "Not enough Jets", jets.size());
//         return 0;
//
//     }
//
//     for (auto & Jet : jets)  {
//         analysis::JetInfo *jet_info = new analysis::JetInfo;
// //         BottomTagger.Branch = BottomTagger.GetBranch(Jet); // FIXME reenable this
//         jet_info->SetBdt(BottomReader.Bdt());
//         Jet.set_user_info(jet_info);
//     }
//
//     std::vector<HHiggsCpv> HiggsCpvs;
//     for (auto Lepton1 = Leptons.begin(); Lepton1 != Leptons.end(); ++Lepton1) {
//       for (auto Lepton2 = Lepton1 + 1; Lepton2 != Leptons.end(); ++Lepton2) {
//         const analysis::Jets eventLeptons {
//                 (*Lepton1), (*Lepton2)
//             };
//             for (auto Jet1 = jets.begin(); Jet1 != jets.end(); ++Jet1) {
//                 for (auto Jet2 = Jet1 + 1; Jet2 != jets.end(); ++Jet2) {
//                     for (auto Jet3 = Jet2 + 1; Jet3 != jets.end(); ++Jet3) {
//                         for (auto Jet4 = Jet3 + 1; Jet4 != jets.end(); ++Jet4) {
//                           const analysis::Jets eventJets = {(*Jet1), (*Jet2), (*Jet3), (*Jet4)};
//                             std::vector<HHiggsCpv> NewHiggsCpvs = GetHiggsCpvs(eventJets, eventLeptons);
//                             HiggsCpvs.insert(HiggsCpvs.end(), NewHiggsCpvs.begin(), NewHiggsCpvs.end());
//                         }
//                     }
//                 }
//             }
//         }
//     }
//
//     std::sort(HiggsCpvs.begin(), HiggsCpvs.end(), SortHiggsCpv());
//
//     EventBranch *eventTagger = static_cast<EventBranch *>(eventBranch->NewEntry());
//
//     eventTagger->ScalarHt = event.Hadrons().ScalarHt();
//     eventTagger->JetNumber = event.Hadrons().Jets().size();
// //     eventTagger->BottomNumber = event.Hadrons().BottomJets().size();
//     eventTagger->LeptonNumber = event.Leptons().leptons().size();
// //     eventTagger->HeavyParticleTag = HiggsCpvs.front().Bdt();
// //     eventTagger->TopDeltaRap = HiggsCpvs.front().GetTopDeltaRap();
// //     eventTagger->TopDeltaPhi = HiggsCpvs.front().GetTopDeltaPhi();
// //     eventTagger->TopDeltaR = HiggsCpvs.front().GetTopDeltaR();
//     if (NewStudyName == "Signal") {
// //         eventTagger->Signal = 1;
//     } else {
// //         eventTagger->Signal = 0;
//     }
//
//     return 1;
//
// }
//
// std::vector< higgscpv::HHiggsCpv > higgscpv::Analysis::GetHiggsCpvs(const analysis::Jets &jets, const analysis::Jets &Leptons)
// {
//
//     std::vector<HHiggsCpv> HiggsCpvs;
//
//     analysis::Doublet HiggsPair01 = analysis::Doublet(jets[0], jets[1]);
// //     HiggsTagger.FillBranch(HiggsPair01);
//     HiggsPair01.SetBdt(HiggsReader.Bdt());
//     analysis::Doublet HiggsPair02 = analysis::Doublet(jets[0], jets[2]);
// //     HiggsTagger.FillBranch(HiggsPair02);
//     HiggsPair02.SetBdt(HiggsReader.Bdt());
//     analysis::Doublet HiggsPair03 = analysis::Doublet(jets[0], jets[3]);
// //     HiggsTagger.FillBranch(HiggsPair03);
//     HiggsPair03.SetBdt(HiggsReader.Bdt());
//     analysis::Doublet HiggsPair12 = analysis::Doublet(jets[1], jets[2]);
// //     HiggsTagger.FillBranch(HiggsPair12);
//     HiggsPair12.SetBdt(HiggsReader.Bdt());
//     analysis::Doublet HiggsPair13 = analysis::Doublet(jets[1], jets[3]);
// //     HiggsTagger.FillBranch(HiggsPair13);
//     HiggsPair13.SetBdt(HiggsReader.Bdt());
//     analysis::Doublet HiggsPair23 = analysis::Doublet(jets[2], jets[3]);
// //     HiggsTagger.FillBranch(HiggsPair23);
//     HiggsPair23.SetBdt(HiggsReader.Bdt());
//
//     analysis::Doublet Top0 = analysis::Doublet(jets[0], Leptons[0]);
// //     LeptonicTopTagger.GetBranch(Top0);
//     Top0.SetBdt(TopReader.Bdt());
//     analysis::Doublet Top1 = analysis::Doublet(jets[1], Leptons[0]);
// //     LeptonicTopTagger.GetBranch(Top1);
//     Top1.SetBdt(TopReader.Bdt());
//     analysis::Doublet Top2 = analysis::Doublet(jets[2], Leptons[0]);
// //     LeptonicTopTagger.GetBranch(Top2);
//     Top2.SetBdt(TopReader.Bdt());
//     analysis::Doublet Top3 = analysis::Doublet(jets[3], Leptons[0]);
// //     LeptonicTopTagger.GetBranch(Top3);
//     Top3.SetBdt(TopReader.Bdt());
//
//     analysis::Doublet AntiTop0 = analysis::Doublet(jets[0], Leptons[1]);
// //     LeptonicTopTagger.GetBranch(AntiTop0);
//     AntiTop0.SetBdt(TopReader.Bdt());
//     analysis::Doublet AntiTop1 = analysis::Doublet(jets[1], Leptons[1]);
// //     LeptonicTopTagger.GetBranch(AntiTop1);
//     AntiTop1.SetBdt(TopReader.Bdt());
//     analysis::Doublet AntiTop2 = analysis::Doublet(jets[2], Leptons[1]);
// //     LeptonicTopTagger.GetBranch(AntiTop2);
//     AntiTop2.SetBdt(TopReader.Bdt());
//     analysis::Doublet AntiTop3 = analysis::Doublet(jets[3], Leptons[1]);
// //     LeptonicTopTagger.GetBranch(AntiTop3);
//     AntiTop3.SetBdt(TopReader.Bdt());
//
//
//     HiggsCpvs.emplace_back(HHiggsCpv(HiggsPair01, Top2, AntiTop3));
//     HiggsCpvs.emplace_back(HHiggsCpv(HiggsPair01, Top3, AntiTop2));
//     HiggsCpvs.emplace_back(HHiggsCpv(HiggsPair02, Top1, AntiTop3));
//     HiggsCpvs.emplace_back(HHiggsCpv(HiggsPair02, Top3, AntiTop1));
//     HiggsCpvs.emplace_back(HHiggsCpv(HiggsPair03, Top1, AntiTop2));
//     HiggsCpvs.emplace_back(HHiggsCpv(HiggsPair03, Top2, AntiTop1));
//     HiggsCpvs.emplace_back(HHiggsCpv(HiggsPair12, Top0, AntiTop3));
//     HiggsCpvs.emplace_back(HHiggsCpv(HiggsPair12, Top3, AntiTop0));
//     HiggsCpvs.emplace_back(HHiggsCpv(HiggsPair13, Top0, AntiTop2));
//     HiggsCpvs.emplace_back(HHiggsCpv(HiggsPair13, Top2, AntiTop0));
//     HiggsCpvs.emplace_back(HHiggsCpv(HiggsPair23, Top0, AntiTop1));
//     HiggsCpvs.emplace_back(HHiggsCpv(HiggsPair23, Top1, AntiTop0));
//
//     return HiggsCpvs;
//
// }
