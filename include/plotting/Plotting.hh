/**
 * Copyright (C) 2015 Jan Hajer
 */
#pragma once

#include <functional>

#include "Tagger.hh"
#include "physics/Rectangle.hh"

class TFile;
class TTree;

namespace boca
{

class Plots;
class Result;
class Results;
class Result;
class Results;
class Plot;

/**
 * @brief Presents result of multivariant analysis
 *
 */
class Plotting
{

public:

    /**
     * @brief Constructor
     *
     */
    Plotting(boca::Tagger& tagger);

    void TaggingEfficiency() const;

    void OptimalCuts() const;

    void RunPlots() const;

//     void Cuts() const;

private:

    Results ReadBdtFiles() const;

    std::vector<Result> ReadBdtFile(TFile& export_file, boca::Tag tag) const;

    Result BdtDistribution(TFile& file, std::string const& tree_name, TFile& export_file) const;

    Result CutDistribution(TFile& file, std::string const& tree_name, TFile& export_file) const;

    void DoPlot(Plots& signals, Plots& backgrounds, boca::Stage stage) const;

    void PlotDetails(boca::Plot& signal, boca::Plot& background, Stage stage) const;

    std::vector<Plots> Import(Stage stage, boca::Tag tag) const;

    Plots PlotResult(TFile& file, std::string const& tree_name, Stage stage) const;

    Plot ReadTree(TTree& tree, std::string const& leaf_1_name, std::string const& leaf_2_name, Stage stage) const;

    void PlotHistogram(const boca::Plot& signal, const boca::Plot& background, Rectangle<float> const& bounds) const;

    void PlotProfile(const boca::Plot& signal, const boca::Plot& background, Rectangle<float> const& bounds) const;

    void PlotAcceptanceGraph(const boca::Results& results) const;

    std::string PlotCrosssectionsGraph(const boca::Results& results) const;

    std::string PlotCrosssectionGraph(Results& results) const;

    std::string PlotHistograms(boca::Results& results) const;

    boca::InfoBranch InfoBranch(TFile& file, std::string const& tree_name) const;

    std::string BestValueTable(Results const& results) const;

    std::string BestValueRow(Results const& results, int bin, std::string const& name) const;

    std::string EfficienciesTable(Results const& results, int bin) const;

    std::string EfficienciesRow(const Result& result, int index, boca::Tag tag, int bin) const;

    std::string PlotEfficiencyGraph(const boca::Results& results) const;

    std::string PlotModelDependentGraph(boca::Results& results) const;

    std::string PlotModelIndependentGraph(boca::Results& results) const;

    void SetBranch(TTree& tree, int& value, std::string const& name) const;

    void SetBranch(TTree& tree, std::vector<float>& values, std::string const& name) const;

    boca::Tagger const& Tagger() const;

    boca::Tagger& tagger_;

//     std::string BestValueTable(Results const& results) const;

//     std::string BestValueRow(Results const& results, int bin, std::string const& name) const;






//     Results ReadCutFiles() const;
//
//     std::vector<Result> ReadCutFile(TFile& export_file, boca::Tag tag) const;
//
//     std::string PlotResult(boca::Results& results) const;
//
//     std::string PlotCutEfficiencyGraph(const Results& results) const;

//     std::string EfficienciesTable(Results const& results, int bin) const;

//     std::string EfficienciesRow(const Result& result, int index, boca::Tag tag, int bin) const;

//     std::string PlotModelIndependentGraph(Results& results) const;

};

}
