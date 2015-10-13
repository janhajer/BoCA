/**
 * Copyright (C) 2015 Jan Hajer
 */
#include "Trainer.hh"

#include "TClonesArray.h"
#include "TFile.h"

#include "TMVA/Config.h"
#include "TMVA/MethodBDT.h"
#include "TMVA/Ranking.h"

#include "Types.hh"
#include "Tagger.hh"
#include "Options.hh"
#include "Debug.hh"

namespace boca
{

Trainer::Trainer(boca::Tagger& tagger, TMVA::Types::EMVA mva) : tagger_(tagger) , factory_(tagger.Name(), &OutputFile(), FactoryOptions())
{
    Error();
    AddVariables();
    PrepareTrainingAndTestTree(GetTrees());
//     TMVA::MethodBase& method =
    BookMethod(mva);
//     const TMVA::Ranking& rank = *method.CreateRanking();
//     rank.SetContext("test");
    Factory().TrainAllMethods();
//     rank.Print();
    Factory().TestAllMethods();
    Factory().EvaluateAllMethods();
}

std::string Trainer::FactoryOptions()
{
    Options options("!Color");
//     options.Add("V");
//     options.Add("!Silent");
    options.Add("!DrawProgressBar");
    return options.str();
}

TFile& Trainer::OutputFile() const
{
    return *TFile::Open(Tagger().FactoryFileName().c_str(), "Recreate");
}

void Trainer::AddVariables()
{
    Note();
    TMVA::gConfig().GetIONames().fWeightFileDir = Tagger().AnalysisName();
    TMVA::gConfig().GetIONames().fWeightFileExtension = Tagger().WeightFileExtension();
    for (auto const & observable : Tagger().Variables()) Factory().AddVariable(observable.expression(), observable.title(), observable.unit(), observable.type());
    for (auto const & spectator : Tagger().Spectators()) Factory().AddSpectator(spectator.expression(), spectator.title(), spectator.unit(), spectator.type());
}

long Trainer::GetTrees()
{
    Note();
    return std::min(GetTree(Tag::signal), GetTree(Tag::background)) / 2;
}

long Trainer::GetTree(Tag tag)
{
    long number = 0;
    for (auto const & tree_name : Tagger().TreeNames(tag)) number += AddTree(tree_name, tag);
    return number;
}

long Trainer::AddTree(std::string const& tree_name, Tag tag)
{
    Debug(tree_name, Name(tag));
    TTree& tree = Tree(tree_name, tag);
    exroot::TreeReader tree_reader = TreeReader(tree_name, tag);
    float weight = Weight(tree_reader);
    Note(weight);
    switch (tag) {
    case Tag::signal :
        Factory().AddSignalTree(&tree, weight);
        break;
    case Tag::background :
        Factory().AddBackgroundTree(&tree, weight);
        break;
    }
    return Entries(tree_reader);
}

exroot::TreeReader Trainer::TreeReader(std::string const& tree_name, Tag tag)
{
    return exroot::TreeReader(&Tree(tree_name, tag));
}

long Trainer::Entries(exroot::TreeReader& tree_reader)
{
    long entries = 0;
    TClonesArray& clones_array = *tree_reader.UseBranch(Tagger().BranchName(Stage::trainer).c_str());
    for (auto const & entry : Range(tree_reader.GetEntries())) {
        tree_reader.ReadEntry(entry);
        entries += clones_array.GetEntries();
    }
    return entries;
}

float Trainer::Weight(exroot::TreeReader& tree_reader)
{
    TClonesArray& clones_array = *tree_reader.UseBranch(Tagger().WeightBranchName().c_str());
    tree_reader.ReadEntry(0);
    return static_cast<InfoBranch&>(*clones_array.First()).Crosssection / tree_reader.GetEntries();
}

TTree& Trainer::Tree(std::string const& tree_name, Tag tag)
{
    Note(Tagger().FileName(Stage::trainer, tag));
    if (!Exists(Tagger().FileName(Stage::trainer, tag).c_str())) Error("File not found", Tagger().FileName(Stage::trainer, tag));
    TFile& file = *TFile::Open(Tagger().FileName(Stage::trainer, tag).c_str());
    if (!file.GetListOfKeys()->Contains(tree_name.c_str())) Error("no tree");
    return static_cast<TTree&>(*file.Get(tree_name.c_str()));
}

void Trainer::PrepareTrainingAndTestTree(long event_number)
{
    Error();
    Options options("SplitMode", "Block");
    options.Add("nTrain_Background", event_number);
    options.Add("nTest_Background", event_number);
    options.Add("nTrain_Signal", event_number);
    options.Add("nTest_Signal", event_number);
    Factory().PrepareTrainingAndTestTree(Tagger().Cut(), Tagger().Cut(), options.str());
}

TMVA::MethodBase& Trainer::BookMethod(TMVA::Types::EMVA mva)
{
    Note();
    return *Factory().BookMethod(mva, Tagger().MethodName(mva), MethodOptions(mva));
}
std::string Trainer::MethodOptions(TMVA::Types::EMVA mva)
{
    Options options;
    switch (mva) {
    case TMVA::Types::EMVA::kBDT :
        options.Add("NTrees", 1000);
        options.Add("MinNodeSize", 2.5, "%");
        options.Add("MaxDepth", 3);
        options.Add("BoostType", "AdaBoost");
        options.Add("AdaBoostBeta", 0.5);
        options.Add("UseBaggedBoost");
        options.Add("BaggedSampleFraction", 0.5);
        options.Add("SeparationType", "GiniIndex");
        options.Add("nCuts", 20);
//         options.Add("VarTransform", "D");
//         options.Add("MinNodeSize", 1.5, "%");
//         options.Add("BoostType", "Grad");
//         options.Add("Shrinkage", 0.10);
//         options.Add("UseBaggedGrad");
//         options.Add("UseRandomisedTrees");
//         options.Add("GradBaggingFraction", 0.5);
//         options.Add("MaxDepth", 4);
//         options.Add("CreateMVAPdfs");
//         options.Add("DoBoostMonitor");
        break;
    default : break;
    }
    return options.str();
}

}
