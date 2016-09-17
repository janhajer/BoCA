#pragma once

// Include the cut analysis base class
#include "boca/analysis/Cut.hh"

/**
* @ingroup Examples
* @brief Cut Example
*
*/
namespace cut
{

// The analysis inherits from the boca cut analysis base class
class Analysis : public boca::analysis::Cut
{

public:

    // Set files for signal and background
    // it is assumed that all files are in the same folder
    void SetFiles(boca::Tag tag, boca::Stage) override
    {
        switch (tag) {
        case boca::Tag::signal :
            // put your signal file here
            this->NewFile(tag, "hh_14TeV-500GeV", "h");
            break;
        case boca::Tag::background :
            // put your background file here
            this->NewFile(tag, "bb_14TeV-500GeV", "bb");
            break;
        }
    }

    //  Usually it is assumed that you call the program from the folder contanining the signal and background files
    //  if this is not the case you have to add the relative path to the working path
    std::string FilePath() const override {
        return this->WorkingPath() + "./";
    }

    // define how many events are supposed to beeing used
    long TrainNumberMax() const override
    {
        return 1000;
    }

    // define the analysis name
    std::string Name() const override
    {
        return "CutAnalysis";
    }

    // Define the cuts
    bool PassPreCut(boca::Event const& event) const override{
        // The event class contains all relevant physical objects
        auto jets = event.Jets();
        if (jets.size() < 2) return false;
        return true;
    }

};

}
