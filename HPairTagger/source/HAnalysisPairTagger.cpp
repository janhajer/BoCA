# include "HAnalysisPairTagger.hh"
# include "Predicate.hh"

hpairtagger::HAnalysis::HAnalysis()
{
    //     DebugLevel = hanalysis::HObject::HDebug;
    Print(HNotification, "Constructor");
    BottomTagger.SetAnalysisName(ProjectName());
    JetPairTagger.SetAnalysisName(ProjectName());
    mkdir(ProjectName().c_str(), 0700);
}

std::string hpairtagger::HAnalysis::StudyName(const hanalysis::HAnalysis::HTagger Tagger) const
{
    Print(HNotification, "Get Study Names", Tagger);

    switch (Tagger) {
    case  HBottomTagger :
        return "Bottom";
    case  HBottomReader :
        return "BottomReader";
    case  HJetPairTagger :
        return "JetPair";
    case  HJetPairReader :
        return "JetPairReader";
    default :
        Print(HError, "unexpected TaggerName", Tagger);
        return "";
    }
}

std::vector<hanalysis::HFile> hpairtagger::HAnalysis::Files(const hanalysis::HAnalysis::HTagger Tagger, const hanalysis::HObject::HTag Tag)
{
    Print(HNotification, "Set File Vector", Tagger, Tag);

    std::vector<hanalysis::HFile> SignalLeptonicFiles;
    std::vector<hanalysis::HFile> BackgroundLeptonicFiles;
    std::vector<hanalysis::HFile> SignalSemiFiles;
    std::vector<hanalysis::HFile> BackgroundSemiFiles;


    SignalSemiFiles.push_back(BackgroundFile(bb,VBF));
//     SignalSemiFiles.push_back(hanalysis::HFile(NameString(bb), SignalCrosssection()));
//     SignalSemiFiles.push_back(hanalysis::HFile(SignalName(Hbb), SignalCrosssection()));

    BackgroundSemiFiles.push_back(BackgroundFile(bb,DYP));
    BackgroundSemiFiles.push_back(BackgroundFile(cc,DYP));
    BackgroundSemiFiles.push_back(BackgroundFile(cc,VBF));
    BackgroundSemiFiles.push_back(BackgroundFile(jj,DYP));
    BackgroundSemiFiles.push_back(BackgroundFile(jj,VBF));
//     BackgroundSemiFiles.push_back(BackgroundFile(ttbb));
//     BackgroundSemiFiles.push_back(BackgroundFile(ttcc));
//     BackgroundSemiFiles.push_back(BackgroundFile(ttjj));
//     BackgroundSemiFiles.push_back(BackgroundFile(ttqq));
//     BackgroundSemiFiles.push_back(BackgroundFile(ttgg));

    std::vector<hanalysis::HFile> SignalHadronicFiles;

    std::vector<hanalysis::HFile> BackgroundHadronicFiles;

    std::vector<hanalysis::HFile> LeptonicFiles = JoinFiles(SignalLeptonicFiles, BackgroundLeptonicFiles);
    std::vector<hanalysis::HFile> HadronicFiles = JoinFiles(SignalHadronicFiles, BackgroundHadronicFiles);
    std::vector<hanalysis::HFile> SemiFiles = JoinFiles(SignalSemiFiles, BackgroundSemiFiles);

    std::vector<hanalysis::HFile> NotLeptonicFiles = JoinFiles(HadronicFiles, SemiFiles);
    std::vector<hanalysis::HFile> CombinedFiles = JoinFiles(NotLeptonicFiles, LeptonicFiles);

    std::vector<hanalysis::HFile> NonLeptonicSignalFiles = JoinFiles(SignalLeptonicFiles, SignalSemiFiles);
    std::vector<hanalysis::HFile> CombinedSignalFiles = JoinFiles(SignalHadronicFiles, NonLeptonicSignalFiles);

    std::vector<hanalysis::HFile> NewFiles;

    switch (Tagger) {
    case  HBottomTagger :
        switch (Tag) {
        case HObject::HSignal :
            NewFiles = SemiFiles;
            break;
        case HObject::HBackground :
            NewFiles = SemiFiles;
            break;
        }
        break;
    case  HBottomReader :
        switch (Tag) {
        case HObject::HSignal :
            NewFiles = SignalSemiFiles;
            break;
        case HObject::HBackground :
            NewFiles = BackgroundSemiFiles;
            break;
        }
        break;
    case  HJetPairTagger :
        switch (Tag) {
        case HObject::HSignal :
            NewFiles = SignalSemiFiles;
            break;
        case HObject::HBackground :
            NewFiles = BackgroundSemiFiles;
            break;
        }
        break;
    case  HJetPairReader :
        switch (Tag) {
        case HObject::HSignal :
            NewFiles = SignalSemiFiles;
            break;
        case HObject::HBackground :
            NewFiles = BackgroundSemiFiles;
            break;
        }
        break;
    default:
        Print(HError, "Files", "unknown tagger name", Tagger);
    }

    NewFiles.front().SetBasePath("~/Projects/PairTagging/");
    NewFiles.front().SetFileSuffix(".root");
    SetTrees(Tagger, Tag);
    PrepareReader(Tagger, Tag);
    return NewFiles;

}


void hpairtagger::HAnalysis::SetTrees(const hanalysis::HAnalysis::HTagger Tagger, const hanalysis::HAnalysis::HTag Tag)
{

    HStrings SignalLeptonicTrees {};
    HStrings BackgroundLeptonicTrees {};



    HStrings SignalSemiTrees {
      TreeName(bb,VBF),
//       SignalTreeName(Hbb)
    };

    HStrings BackgroundSemiTrees {
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

    HStrings SignalHadronicTree {};
    HStrings BackgroundHadronicTrees {};

    HStrings LeptonicTrees = JoinHStrings(SignalLeptonicTrees, BackgroundLeptonicTrees);
    HStrings HadronicTrees = JoinHStrings(SignalHadronicTree, BackgroundHadronicTrees);
    HStrings SemiTrees = JoinHStrings(SignalSemiTrees, BackgroundSemiTrees);

    HStrings NotLeptonicTrees = JoinHStrings(HadronicTrees, SemiTrees);
    HStrings CombinedTrees = JoinHStrings(NotLeptonicTrees, LeptonicTrees);

    switch (Tagger) {
    case HBottomTagger:
        BottomTagger.SetSignalTreeNames(SemiTrees);
        BottomTagger.SetBackgroundTreeNames(SemiTrees);
        if (Tag == HSignal) {
            BottomTagger.SetTagger();
        }
        break;
    case HBottomReader:
        if (Tag == HSignal) {
            BottomTagger.SetTagger();
        }
        BottomTagger.SetSignalTreeNames(SemiTrees);
        BottomTagger.SetBackgroundTreeNames(SemiTrees);
        break;
    case HJetPairTagger :
        JetPairTagger.SetSignalTreeNames(SignalSemiTrees);
        JetPairTagger.SetBackgroundTreeNames(BackgroundSemiTrees);
        if (Tag == HSignal) {
            JetPairTagger.SetTagger(BottomTagger);
        }
        break;
    case HJetPairReader :
        if (Tag == HSignal) {
            JetPairTagger.SetTagger(BottomTagger);
        }
        JetPairTagger.SetSignalTreeNames(SignalSemiTrees);
        JetPairTagger.SetBackgroundTreeNames(BackgroundSemiTrees);
        break;
    default :
        Print(HError, "SetTrees", "unhandeled case");
    }
}

void hpairtagger::HAnalysis::PrepareReader(const hanalysis::HAnalysis::HTagger Tagger, const HTag Tag)
{
    Print(HInformation, "Prepare Reader", Tagger);
    if (Tag == HBackground) {
        return;
    }
    switch (Tagger) {
    case HBottomTagger:
        break;
    case HBottomReader:
        BottomReader.SetMva(BottomTagger);
        break;
    case HJetPairTagger :
        JetPairTagger.BottomTagger.SetTagger();
        JetPairTagger.BottomReader.SetMva(JetPairTagger.BottomTagger);
        break;
    case HJetPairReader :
        BottomReader.SetMva(BottomTagger);
        JetPairReader.SetMva(JetPairTagger);
        break;
    default :
        Print(HError, "PrepareReader", "unhandled case");
    }
}

void hpairtagger::HAnalysis::NewBranches(ExRootTreeWriter &NewTreeWriter, const hanalysis::HAnalysis::HTagger Tagger)
{
    Print(HNotification, "New Branches", Tagger);

    switch (Tagger) {
    case HBottomTagger :
        Branch = NewTreeWriter.NewBranch(StudyName(Tagger).c_str(), HBottomBranch::Class());
        break;
    case HBottomReader :
        Branch = NewTreeWriter.NewBranch(StudyName(Tagger).c_str(), HBottomBranch::Class());
        break;
    case HJetPairTagger :
        Branch = NewTreeWriter.NewBranch(StudyName(Tagger).c_str(), HEventJetPairBranch::Class());
        break;
    case HJetPairReader :
        Branch = NewTreeWriter.NewBranch(StudyName(Tagger).c_str(), HEventJetPairBranch::Class());
        break;
    default :
        Print(HError, "No Branch filled");
    }

}

bool hpairtagger::HAnalysis::Analysis(hanalysis::HEvent &Event, const hanalysis::HAnalysis::HTagger Tagger, const HTag Tag)
{
    Print(HInformation, "Analysis", Tagger);
    ++event_sum_;

    switch (Tagger) {
    case HBottomTagger :
        return GetBottomTag(Event, Tag);
    case HBottomReader:
        return GetBottomReader(Event, Tag);
    case HJetPairTagger :
        return GetJetPairTag(Event, Tag);
    case HJetPairReader :
        return GetJetPairReader(Event, Tag);
    default :
        Print(HError, "unknown Tagger", Tagger);
        return 0;
    }
}


bool hpairtagger::HAnalysis::GetBottomTag(hanalysis::HEvent &Event, const HTag Tag)
{
    Print(HDebug, "Get Bottom Tag", Tag);
    std::vector<HBottomBranch> Bottoms = BottomTagger.GetBranches(Event, Tag);
    if (Bottoms.size() < 1) {
        return 0;
    }
    for (const auto & Bottom : Bottoms) {
        *static_cast<HBottomBranch *>(Branch->NewEntry()) = Bottom;
    }
    return 1;
}

bool hpairtagger::HAnalysis::GetBottomReader(hanalysis::HEvent &Event, const HTag Tag)
{
    Print(HDebug, "Get Bottom Reader", Tag);
    HJets Jets = BottomTagger.GetJets(Event);
    Jets = BottomTagger.GetJetBdt(Jets, BottomReader);
    if (Jets.size() < 1) {
        return 0;
    }

    HJets Particles = Event.GetParticles()->Generator();
    Particles.erase(std::remove_if(Particles.begin(), Particles.end(), WrongAbsId(BottomId)), Particles.end());

    for (const auto & Particle : Particles) {
        std::sort(Jets.begin(), Jets.end(), MinDeltaR(Particle));
        if (Jets.front().delta_R(Particle) < BottomTagger.DetectorGeometry.JetConeSize) {
            static_cast<hanalysis::HJetInfo *>(Jets.front().user_info_shared_ptr().get())->SetTag(HSignal);
        }
    }

    for (const auto & Jet : Jets) {
        if (Tag != Jet.user_info<hanalysis::HJetInfo>().Tag()) {
            continue;
        }
//         if (std::abs(Jet.rap()) > BottomTagger.DetectorGeometry.TrackerEtaMax) {
//             continue;
//         }
        *static_cast<HBottomBranch *>(Branch->NewEntry()) = BottomTagger.GetBranch(Jet);
    }
    return 1;
}

bool hpairtagger::HAnalysis::GetJetPairTag(hanalysis::HEvent &Event, const HTag Tag)
{
    Print(HDebug, "Get JetPair Tag", Tag);
    std::vector<HEventJetPairBranch> JetPairs = JetPairTagger.GetBranches(Event, Tag, MotherId(ProductionChannel()));
    if (JetPairs.size() < 1) {
        return 0;
    }
    for (const auto & JetPair : JetPairs) {
        *static_cast<HEventJetPairBranch *>(Branch->NewEntry()) = JetPair;
    }
    return 1;
}

bool hpairtagger::HAnalysis::GetJetPairReader(hanalysis::HEvent &Event, const HTag Tag)
{
    Print(HDebug, "Get JetPair Reader", Tag);
    HJets Jets = BottomTagger.GetJets(Event);
    Jets = BottomTagger.GetJetBdt(Jets, BottomReader);
    if (Jets.size() < 1) {
        Print(HInformation, "just one jet", Jets.size());
        return 0;
    }
    Print(HError, "jets", Jets.size());

//     HJets FilteredJets; // WRONG MUST BE REMOVED
    HJets Particles = Event.GetParticles()->Generator();
    if (Tag == HSignal) {
//         Particles = BottomTagger.RemoveIfWrongAbsFamily(Particles, BottomId, MotherId(ProductionChannel()));
    }
    if (
// ProductionChannel() == Associated &&
        Tag == HBackground) {
//         Particles = RemoveIfWrongAbsStepMother(Particles, TopId); // THIS IS WRONG AND SHOULD BE REMOVED AGAIN
//         Particles = BottomTagger.RemoveIfWrongParticle(Particles, GluonId); // THIS IS WRONG AND SHOULD BE REMOVED AGAIN
//         Particles = BottomTagger.RemoveIfWrongAbsMother(Particles, ZId); // THIS IS WRONG AND SHOULD BE REMOVED AGAIN
    }

//     if (
//         Tag == HSignal &&
//         Particles.size() != 2) {
//         Print(HError, "where are the quarks?", Particles.size());
//     }

//     for (const auto & Particle : Particles) {
//         std::sort(Jets.begin(), Jets.end(), MinDeltaR(Particle));
//         if (Jets.front().delta_R(Particle) < BottomTagger.DetectorGeometry.JetConeSize) {
//             static_cast<hanalysis::HJetInfo *>(Jets.front().user_info_shared_ptr().get())->SetTag(HSignal);
//             FilteredJets.push_back(Jets.front()); // WRONG MUST BE REMOVED
//         }
//     }

//     std::vector<hanalysis::HDoublet> Doublets = JetPairTagger.GetBdt(FilteredJets, JetPairReader);
    std::vector<hanalysis::HDoublet> Doublets = JetPairTagger.GetBdt(Jets, JetPairReader);
    if (Doublets.size() > 1) {
        std::sort(Doublets.begin(), Doublets.end());
        Doublets.erase(Doublets.begin() + 1, Doublets.end());
    }

    for (const auto & Doublet : Doublets) {
        static_cast<HEventJetPairBranch &>(*Branch->NewEntry()) = JetPairTagger.GetBranch(Doublet);
    }
    return 1;
}