/**
 * Copyright (C) 2015-2016 Jan Hajer
 */
#pragma once

#include "TFile.h"

#include "TMVA/Factory.h"
// #include "TMVA/DataLoader.h"

#include "boca/Tag.hh"

namespace boca {

class TaggerBase;

/**
 * @brief Prepares multivariant analysis
 *
 */
class Trainer {

public:

    /**
     * @brief Constructor
     *
     */
    Trainer(TaggerBase& tagger);

private:

    /**
     * @brief Book MVA methods
     *
     */
    std::vector<double> BookMethod();

    /**
     * @brief Add Variables
     */
    void AddObservables();

    /**
     * @brief Get Trees
     *
     */
    long AddAllTrees();

    long AddTrees(Tag tag);

    /**
     * @brief Prepare Trainig and Test Trees
     *
     */
    void PrepareTrainingAndTestTree(long event_number);

    long AddTree(std::string const& tree_name, Tag tag);

    long Entries(std::string const& tree_name, Tag tag);

    double Weight(std::string const& tree_name, Tag tag);

    std::string FactoryOptions();

    std::string DataLoaderOptions();

    std::string MethodOptions() const;

    TaggerBase const& Tagger() const;

    TaggerBase & Tagger();

    TMVA::Factory const& Factory() const;

    TMVA::Factory& Factory();

    TaggerBase& tagger_;

    TFile output_;

    TMVA::Factory factory_;

//     TMVA::DataLoader data_loader_;

    TMVA::Factory & DataLoader(){
      return factory_;
    }

    std::map<Tag, TFile> input_;

};

}
