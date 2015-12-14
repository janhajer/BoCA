#pragma once

#include <vector>

#include "TGraph.h"
#include "TMultiGraph.h"
#include "TLine.h"

#include "plotting/Legend.hh"
#include "plotting/Canvas.hh"

namespace boca
{

class Graphs : public Canvas
{

public:

    Graphs(std::string const& path, std::string const& name, bool show_title = false);

    ~Graphs();

    void AddGraph(std::vector<float> const& xs, std::vector<float> const& ys, std::string const& name = "");

    void SetLegend(Orientation orientation, std::string const& title = "");

    void Draw();

    void SetXAxis(std::string const& title, Bounds<float> const& bounds = Bounds<float>());

    void SetYAxis(std::string const& title, Bounds<float> const& bounds = Bounds<float>());

    void AddLine(float x_value);

private:

    Bounds<double> LimitsY();

    void AddGraphs();

    TMultiGraph multi_graph_;

    Legend legend_;

    std::vector<TGraph> graphs_;

    std::vector<TLine> lines_;

};

}
