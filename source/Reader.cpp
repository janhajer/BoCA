# include "Reader.hh"

# include "TClonesArray.h"
# include "TPad.h"
# include "TDirectoryFile.h"
# include "TTree.h"
# include "TCanvas.h"
# include "TMultiGraph.h"
# include "TLine.h"
# include "TStyle.h"
# include "TH1F.h"

// # include "TMVA/Reader.h"
# include "TMVA/MethodCuts.h"

namespace analysis
{

Result::Result()
{
    steps = 20000;
    events.resize(steps, 0);
    efficiency.resize(steps, 0);
    analysis_event_number.resize(steps, 0);
    bins.resize(steps, 0);
}

std::vector<int> Result::CutIntegral() const
{
    std::vector<int> integrals(steps, 0);
    integrals.at(steps - 1) = bins.at(steps - 1);
    for (int step = steps - 2; step >= 0; --step) integrals.at(step) = integrals.at(step + 1) + bins.at(step);
    return integrals;
}

Reader::Reader()
{
//     debug_level_ = kDebug;
    Print(kInformation, "Constructor");
}

Reader::Reader(Tagger &tagger)
{
//     debug_level_ = kDebug;
    Print(kInformation, "Constructor with tagger");
    SetTagger(tagger);
}

void Reader::SetTagger(Tagger &tagger)
{
    Print(kNotification, "SetMva");
    tagger_ = &tagger;
    AddVariable();
    BookMva();
}

void Reader::AddVariable()
{
    Print(kNotification, "Add Variable");
    const std::string default_options = "!Color:Silent";
    for (auto & observable : tagger().observables()) reader().AddVariable(observable.expression(), observable.value());
    for (auto & spectator : tagger().spectators()) reader().AddSpectator(spectator.expression(), spectator.value());

}

void Reader::BookMva()
{
    Print(kNotification, "Book Mva");
    const std::string bdt_weight_file = tagger().analysis_name() + "/" + tagger().bdt_weight_name();
    Print(kNotification, "Opening Weight File", bdt_weight_file);
    reader().BookMVA(tagger().bdt_method_name(), bdt_weight_file);
}

float Reader::Bdt() const
{
    Print(kInformation, "Bdt");
    return const_cast<TMVA::Reader &>(reader_).EvaluateMVA(tagger().bdt_method_name()) + 1;
}

Results Reader::ExportFile() const
{
    std::string export_file_name = tagger().analysis_name() + "/" + tagger().analysis_name() + ".root";
    TFile export_file(export_file_name.c_str(), "Recreate");

    Results results;

    std::string signal_file_name = tagger().analysis_name() + "/" + tagger().signal_name() + "Reader.root";
    results.signal = Export(export_file, signal_file_name, tagger().signal_tree_names());

    std::string background_file_name = tagger().analysis_name() + "/" + tagger().background_name() + "Reader.root";
    results.background = Export(export_file, background_file_name, tagger().background_tree_names());

    export_file.Close();

    return results;
}

std::vector<Result> Reader::Export(TFile &export_file, const std::string &file_name, const Strings &treename) const
{
    TFile file(file_name.c_str(), "Read");
    Print(kError, "Open Signal File", file_name, treename.size());

    std::vector<Result> results;
    for (const auto & tree_name : treename) results.emplace_back(BdtResult(file, tree_name, export_file));

    return results;
}

TLegend Reader::Legend(float x_min, float y_max, float width, float height, const std::string &name)
{
    TLegend legend(x_min, y_max - height, x_min + width, y_max);
    legend.SetHeader(name.c_str());
    legend.SetBorderSize(0);
    legend.SetFillStyle(0);
    return legend;
}

void Reader::PlotHistograms(const Results &results)
{
    Print(kError, "PlotHistograms");
    gStyle->SetOptStat("");

    TCanvas canvas;
    std::vector<TH1F> histograms;
    TLegend legend = Legend(0.15, 0.85, 0.2, 0.4);

    float x_min = 0;
    for (const auto & result : results.background)  {
        float min = *std::min_element(result.bdt.begin(), result.bdt.end());
        min -= 1;
        if (min < x_min) x_min = min;
    }

    float x_max = 0;
    for (const auto & result : results.signal)  {
        float max = *std::max_element(result.bdt.begin(), result.bdt.end());
        max -= 1;
        if (max > x_max) x_max = max;
    }

    float y_max = 0;
    for (const auto & result : results.signal)  {
        TH1F histogram(result.info_branch.Name.c_str(), "", 50, x_min * 1.1, x_max * 1.1);
        for (const float & bdt : result.bdt) histogram.Fill(bdt - 1);
        float max = histogram.GetBinContent(histogram.GetMaximumBin());
        if (max > y_max) y_max = max;
        histograms.emplace_back(histogram);
    }

    for (const auto & result : results.background)  {
        TH1F histogram(result.info_branch.Name.c_str(), "", 50, x_min * 1.1, x_max * 1.1);
        for (const float & bdt : result.bdt) histogram.Fill(bdt - 1);
        histogram.SetLineColor(ColorCode(&result - &results.background[0] + 1));
        histogram.SetLineStyle(&result - &results.background[0] + 2);
        histograms.emplace_back(histogram);
        float max = histogram.GetBinContent(histogram.GetMaximumBin());
        if (max > y_max) y_max = max;
    }

    Strings nice_names;
    for (const auto & result : results.signal) nice_names.emplace_back(result.info_branch.Name);
    for (const auto & result : results.background) nice_names.emplace_back(result.info_branch.Name);
    for (auto & histogram : histograms) {
        histogram.SetAxisRange(0, y_max * 1.05, "Y");
        histogram.GetXaxis()->SetTitle("BDT");
        histogram.GetYaxis()->SetTitle("N");
        std::string name = nice_names.at(&histogram - &histograms[0]);
        legend.AddEntry(&histogram, name.c_str(), "l");
        histogram.Draw("same");
    }
    legend.Draw();

    const std::string efficiency_file_name = tagger().analysis_name() + "-Bdt.pdf";
    const std::string efficiency_file_path = tagger().analysis_name() + "/" + efficiency_file_name;
    canvas.Print(efficiency_file_path.c_str());
}

void Reader::PlotMultiGraph(const Results &results)
{
    Print(kError, "PlotMultiGraph");
    TCanvas canvas;
    TMultiGraph multi_graph;
    std::vector<TGraph> graphs;
    for (const auto & result : results.background) graphs.emplace_back(TGraph(result.steps, &results.signal.front().efficiency[0], &result.efficiency[0]));
    Strings nice_names;
    for (const auto & result : results.background) nice_names.emplace_back(result.info_branch.Name);
    canvas.SetLogy();
    TLegend legend = Legend(0.15, 0.85, 0.2, 0.4, results.signal.front().info_branch.Name);
    for (auto & graph : graphs) {
        graph.SetLineColor(ColorCode(&graph - &graphs[0]));
        graph.SetLineStyle(&graph - &graphs[0] + 1);
        multi_graph.Add(&graph);
        std::string name = nice_names.at(&graph - &graphs[0]);
        legend.AddEntry(&graph, name.c_str(), "l");
    }
    multi_graph.Draw("al");
    multi_graph.GetXaxis()->SetLimits(0.2, 0.9);
    multi_graph.GetXaxis()->SetTitle("Signal acceptance");
    multi_graph.GetYaxis()->SetTitle("Background acceptance");
    multi_graph.SetMaximum(1);
    multi_graph.SetMinimum(0.01);
    legend.Draw();

    std::string efficiency_file_name = tagger().analysis_name() + "-Acceptance.pdf";
    std::string efficiency_file_path = tagger().analysis_name() + "/" + efficiency_file_name;
    canvas.Print(efficiency_file_path.c_str());
}

void Reader::TaggingEfficiency()
{
    Print(kNotification, "Tagging Efficiency");

    Results results = ExportFile();

    PlotHistograms(results);
    PlotMultiGraph(results);

}

void Reader::OptimalSignificance()
{
    Print(kNotification, "Mva Loop");
    std::stringstream TableHeader;
    TableHeader << "\n\\begin{table}\n\\centering\n\\begin{tabular}{rlll}\n\\toprule\n";

    Results results = ExportFile();

//     const std::string export_file_name = tagger().analysis_name() + "/" + tagger().analysis_name() + ".root";
//     TFile export_file(export_file_name.c_str(), "Recreate");

//     const std::string background_file_name = tagger().analysis_name() + "/" + tagger().background_name() + "Reader.root";
//     TFile background_file(background_file_name.c_str(), "Read");
//     Print(kError, "Open Background File", background_file_name, tagger().background_tree_names().size());


//     std::vector<Result> background_results;
//     for (const auto & background_tree_name : tagger().background_tree_names()) background_results.emplace_back(BdtResult(background_file, background_tree_name, export_file));

//     const std::string signal_file_name = tagger().analysis_name() + "/" + tagger().signal_name() + "Reader.root";
//     TFile signal_file(signal_file_name.c_str(), "Read");
//     Print(kError, "Open Signal File", signal_file_name, tagger().signal_tree_names().size());

//     Result signal_results;
    std::vector<float> Significances(Result().steps, 0);
    std::vector<float> x_values(Result().steps, 0);
//     for (const auto & signal_tree_names : tagger().signal_tree_names()) {
    for (const auto & signal_results : results.signal) {
//         signal_results = BdtResult(signal_file, signal_tree_names, export_file);
        for (int step = 0; step < Result().steps; ++step) {
            float background_events = 0;
            for (const auto & background_result : results.background) background_events += background_result.events[step];
            if (signal_results.events[step] + background_events > 0) Significances.at(step) = signal_results.events[step] / std::sqrt(signal_results.events[step] + background_events);
            else Significances.at(step) = 0;
            x_values.at(step) = float(step) * 2 / Result().steps;
        }
    }
//     export_file.Close();

std::vector<float> background_efficiencies(results.background.size(), 0);
    int BestBin = 0;
    int counter = 0;
    for (std::size_t background_number = 0; background_number < results.background.size(); ++background_number) {
      while (background_efficiencies.at(background_number) == 0 && counter < Result().steps) {
            BestBin = std::distance(Significances.begin(), std::max_element(std::begin(Significances), std::end(Significances) - counter));
            background_efficiencies.at(background_number) = results.background.at(background_number).efficiency.at(BestBin);
            ++counter;
        }
    }

    float MaxSignificance = Significances.at(BestBin);
    float SignalEfficiency = results.signal.front().efficiency.at(BestBin);
//     const analysis::InfoBranch info_branch = InfoBranch(signal_file, tagger().signal_tree_names().front());
    std::stringstream Table;
    Table << TableHeader.str();
    Table << "    Mass\n" << "  & " << results.signal.front().info_branch.Mass;
    Table << "\n \\\\ \\midrule\n";
    Table << "    BDT-cut\n" << "  & " << float(BestBin) * 2 / Result().steps;
    Table << "\n \\\\ $p$-value\n  & " << MaxSignificance;
    Table << "\n \\\\ Efficiency\n  & " << SignalEfficiency << "\n  & " << results.signal.front().analysis_event_number.at(BestBin) << "\n  & " << results.signal.front().event_sum() << "\n";

    for (std::size_t background_number = 0; background_number < results.background.size(); ++background_number) {
      Table << " \\\\ \\verb|" << tagger().background_tree_names().at(background_number) << "|\n  & " << results.background.at(background_number).efficiency.at(BestBin) << "\n  & " << results.background.at(background_number).analysis_event_number.at(BestBin) << "\n  & " << results.background.at(background_number).event_sum() << "\n";
    }

    Result signal_results = results.signal.front();

    TCanvas efficiency_canvas;
    efficiency_canvas.SetLogy();
    TMultiGraph multi_graph;
    std::vector<TGraph> rejection_graphs;
    for (const auto & background_result : results.background) rejection_graphs.emplace_back(TGraph(background_result.steps, &x_values[0], &background_result.efficiency[0]));
    for (auto & rejection_graph : rejection_graphs) multi_graph.Add(&rejection_graph);
    TGraph EfficiencyGraph(signal_results.steps, &x_values[0], &signal_results.efficiency[0]);
    EfficiencyGraph.SetLineColor(kRed);
    multi_graph.Add(&EfficiencyGraph);
    multi_graph.Draw("al");
    TLine EfficiencyLine(float(BestBin) * 2 / signal_results.steps, multi_graph.GetYaxis()->GetXmin(), float(BestBin) * 2 / signal_results.steps, multi_graph.GetYaxis()->GetXmax());
    EfficiencyLine.SetLineStyle(2);
    EfficiencyLine.Draw();
    const std::string efficiency_file_name = tagger().analysis_name() + "-Efficiency.tex";
    const std::string efficiency_file_path = tagger().analysis_name() + "/" + efficiency_file_name;
    efficiency_canvas.Print(efficiency_file_path.c_str());

    TCanvas SignificanceCanvas;
    TGraph SignificanceGraph(signal_results.steps, &x_values[0], &Significances[0]);
    SignificanceGraph.SetTitle("");
    SignificanceGraph.Draw("al");
    SignificanceCanvas.Update();
    TLine SignificanceLine(float(BestBin) * 2 / signal_results.steps, gPad->GetUymin(), float(BestBin) * 2 / signal_results.steps, gPad->GetUymax());
    SignificanceLine.SetLineStyle(2);
    SignificanceLine.Draw();
    const std::string SignificanceFileName = tagger().analysis_name() + "-Significance.tex";
    const std::string SignificanceFilePath = tagger().analysis_name() + "/" + SignificanceFileName;
    SignificanceCanvas.Print(SignificanceFilePath.c_str());

//     TCanvas BdtCanvas;
//     std::vector<TH1F> Histograms;
//     for (const auto & background_result : background_results) {
//       TH1F BackgroundHistogram("","",50,0,2);
//       for(const auto Bdt : background_result.Bdt) BackgroundHistogram.Fill(Bdt);
//       Histograms.emplace_back(BackgroundHistogram);
//     }
//     TH1F SignalHistogram("","",50,0,2);
//     for(const auto Bdt : signal_results.Bdt) SignalHistogram.Fill(Bdt);
//     SignalHistogram.SetLineColor(kRed);
//     Histograms.emplace_back(SignalHistogram);
//     float YMax = 0;
//     for(const auto & Histogram : Histograms) if(Histogram.GetBinContent(Histogram.GetMaximumBin())> YMax) YMax = Histogram.GetBinContent(Histogram.GetMaximumBin());
//     for(auto & Histogram : Histograms) {
//       Histogram.SetAxisRange(0, 1.05 * YMax , "Y");
//     Histogram.Draw("same");
//     }
//     TLine BdtLine(float(BestBin) * 2 / signal_results.steps, 0, float(BestBin) * 2 / signal_results.steps, YMax);
//     BdtLine.SetLineStyle(2);
//     BdtLine.Draw();
//     const std::string BdtFileName = tagger().GetAnalysisName() + "-Bdt.pdf";
//     const std::string BdtFilePath = tagger().GetAnalysisName() + "/" + BdtFileName;
//     BdtCanvas.Print(BdtFilePath.c_str());


    std::stringstream TableFooter;
    TableFooter << " \\\\ \\bottomrule\n\\end{tabular}\n\\caption{Significance and efficiencies.}\n\\end{table}\n";
    Table << TableFooter.str();

    std::ofstream LatexFile;
    LatexHeader(LatexFile);
    LatexFile << Table.str();

    LatexFile << "\n\\begin{figure}\n\\centering\n\\scalebox{0.6}{\\input{" << efficiency_file_name << "}}\n\\caption{Efficiency.}\n\\end{figure}\n";
    LatexFile << "\n\\begin{figure}\n\\centering\n\\scalebox{0.6}{\\input{" << SignificanceFileName << "}}\n\\caption{Significance.}\n\\end{figure}\n";
//     LatexFile << "\n\\begin{figure}\n\\centering\n\\scalebox{0.6}{\\input{" << BdtFileName << "}}\n\\caption{Bdt.}\n\\end{figure}\n";

    LatexFooter(LatexFile);
}

Result Reader::BdtResult(TFile &file, const std::string &tree_name, TFile &export_file) const
{
    Print(kNotification, "Apply Bdt", tree_name);
    const float Luminosity = 3000; // 3000 fb-1

    Print(kError, "Open Tree", tree_name);
    exroot::TreeReader tree_reader(static_cast<TTree *>(file.Get(tree_name.c_str())));
    Result result = BdtDistribution(tree_reader, tree_name, export_file);
    result.info_branch = InfoBranch(file, tree_name);

//     result.event_sum = result.info_branch.EventNumber;
    std::vector<int> Integral = result.CutIntegral();

    for (int step = 0; step < result.steps; ++step) {
        result.events[step] = float(Integral[step]) / float(result.info_branch.EventNumber) * result.info_branch.Crosssection * Luminosity;
        result.efficiency[step] = float(Integral[step]) / float(result.info_branch.EventNumber);
        result.analysis_event_number[step] = Integral[step];
//         result.bdt[step] = bins[step];
        Print(kDebug, "Result", result.efficiency[step], result.events[step]);
    }
    return result;
}

Result Reader::BdtDistribution(exroot::TreeReader &tree_reader, const std::string &tree_name, TFile &export_file) const
{
    Print(kNotification, "Bdt Distribution", tagger().branch_name());
    std::string NeweventBranchName = tagger().branch_name() + "Reader";

    Result result;
    TClonesArray &event_clones_array = *tree_reader.UseBranch(NeweventBranchName.c_str());
    exroot::TreeWriter tree_writer(&export_file, tree_name.c_str());
    exroot::TreeBranch &result_branch = *tree_writer.NewBranch(NeweventBranchName.c_str(), ResultBranch::Class());
    for (const int eventNumber : Range(tree_reader.GetEntries())) {
        tree_reader.ReadEntry(eventNumber);
        for (const int Entry : Range(event_clones_array.GetEntriesFast())) {
            float bdt_value = tagger().ReadBdt(event_clones_array, Entry);
            result.bdt.emplace_back(bdt_value);
            if (bdt_value < 0 || bdt_value > 2) Print(kError, "Bdt Value" , bdt_value);
            static_cast<ResultBranch &>(*result_branch.NewEntry()).Bdt = bdt_value;
//             Print(kNotification, "Bdt Distribution", BdtValue,std::floor(BdtValue * result.steps / 2) - 1);
            int bin = std::floor(bdt_value * result.steps / 2) - 1;
            if (bin == -1) bin = 0; // FIXME clean this up
//             ++bins.at(std::floor(BdtValue * result.steps / 2) - 1);
            ++result.bins.at(bin);
        }
        tree_writer.Fill();
        tree_writer.Clear();
    }
    tree_writer.Write();
    return result;
}

InfoBranch Reader::InfoBranch(TFile &file, const std::string &tree_name) const
{
    Print(kNotification, "Info Branch", tree_name);
    exroot::TreeReader tree_reader(static_cast<TTree *>(file.Get(tree_name.c_str())));
    Print(kError, "Info Branch", tree_name, tagger().weight_branch_name());
    TClonesArray &clones_array = *tree_reader.UseBranch(tagger().weight_branch_name().c_str());
//     tree_reader.ReadEntry(tree_reader.GetEntries() - 1);
    tree_reader.ReadEntry(0);
    return dynamic_cast<analysis::InfoBranch &>(*clones_array.At(0));
}


void Reader::LatexHeader(std::ofstream &latex_file) const
{
    Print(kNotification, "LaTeX Header");
    const std::string TexFileName = tagger().analysis_name() + "/" + tagger().analysis_name() + ".tex";
    latex_file.open(TexFileName);
    latex_file << "\\documentclass[a4paper,10pt]{article}\n\n"
               << "\\usepackage{booktabs}\n"
               << "\\usepackage{graphicx}\n"
               //               << "\\usepackage[landscape]{geometry}\n"
               << "\\usepackage[cm]{fullpage}\n"
               //               << "\\usepackage{units}\n"
               //               << "\\usepackage{siunitx}\n\n"
               //               << "\\newcolumntype{R}{S[table-number-alignment = right, table-parse-only]}\n"
               //               << "\\newcolumntype{L}{S[table-number-alignment = left,table-parse-only]}\n"
               //               << "\\newcolumntype{E}{R@{$\\pm$}L}\n"
               << "\\usepackage{tikz}\n"
               << "\\usetikzlibrary{patterns}\n"
               << "\\usetikzlibrary{plotmarks}\n"
               << "\n\\begin{document}\n";
}


void Reader::LatexFooter(ofstream &latex_file) const
{
    Print(kNotification, "LaTeX Footer");
    latex_file << "\n\\end{document}\n";
    latex_file.close();
}

// float Reader::GetRatio(const float Nominator, const float Denummertor) const
// {
//     float Ratio;
//     if (Denummertor > 0) {
//         Ratio = float(Nominator) / Denummertor;
//     } else {
//         Ratio = 0;
//     }
//     return Ratio;
// }
//
//
// float Reader::GetScaling(const float events, const int Particles) const
// {
//     Print(kInformation , "Scaling");
//     float Scaling;
//     if (Particles == 0) {
//         Scaling = 0;
//     } else {
//         Scaling = events / Particles;
//     }
//     return Scaling;
// }
//
// float Reader::GetLuminosity(const float Number) const
// {
//     Print(kInformation , "Luminosity");
//     float Luminosity = Number / CrosssectionScaled;
//     return Luminosity;
// }
//
// float Reader::GetLuminosityError(const float Number) const
// {
//     Print(kInformation , "Luminosity Error");
//     float LuminosityError = GetError(Number) / CrosssectionScaled + Number / CrosssectionNorm * LuminosityScalingError + GetLuminosity(Number) * CrosssectionNormRelError;
//     return LuminosityError;
// }
//
// float Reader::GetError(const float Value) const
// {
//     Print(kInformation , "Error");
//     float Error;
//     if (Value == 0) {
//         Error = 0;
//     } else {
//         Error = 1 / sqrt(Value);
//     }
//     return Error;
// }
//
// float Reader::RoundToDigits(const float Value) const
// {
//     return RoundToDigits(Value, 3);
// }
//
// float Reader::RoundError(const float Value) const
// {
//     return RoundToDigits(Value, 2);
// }
//
//
// float Reader::RoundToDigits(const float Value, const int Digits) const
// {
//     Print(kInformation , "Round To Digits");
//     if (Value == 0 || Value != Value) {
//         return 0;
//     } else {
//         const float Factor = std::pow(10.0, Digits - ceil(log10(std::abs(Value))));
//         return (round(Value * Factor) / Factor);
//     }
// }
//
// float Reader::RoundToError(const float Value, const float Error) const
// {
//     Print(kInformation , "Round To Digits");
//     if (Value == 0) {
//         return 0;
//     } else {
//         const float Factor = std::pow(10.0, 2 - ceil(log10(std::abs(Error))));
//         return (round(Value * Factor) / Factor);
//     }
// }

int Reader::ColorCode(const int number) const
{
    switch (number) {
    case 0 :
        return kBlack;
    case 1 :
        return kRed;
    case 2 :
        return kBlue;
    case 3 :
        return kTeal - 5;
    case 4 :
        return kPink + 1;
    case 5 :
        return kViolet;
    case 6 :
        return kOrange;
    case 7 :
        return kAzure;
    case 8 :
        return kSpring - 5;
    case 9 :
        return kGreen + 3;
    case 10 :
        return kCyan - 3;
    case 11 :
        return kMagenta - 3;
    case 12 :
        return kYellow - 9;
    case 13 :
        return kGray;
    default :
        return kBlack;
    }
}

}
