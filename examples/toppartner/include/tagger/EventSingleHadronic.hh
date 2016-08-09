/**
 * Copyright (C) 2015-2016 Jan Hajer
 */
#pragma once

#include "boca/tagger/Global.hh"
#include "boca/multiplets/EventMultiplet.hh"

#include "include/tagger/SignatureSingleHadronic.hh"
#include "include/branch/NewEvent2.hh"

namespace toppartner
{

namespace tagger
{

/**
 *
 * @brief Prepares multivariant analysis
 *
 */
class EventSingleHadronic : public Tagger<EventMultiplet<Decuplet532>, branch::NewEvent2>
{

public:

    int Train(boca::Event const& event, PreCuts const& pre_cuts, Tag tag) override;

    std::vector<EventMultiplet<Decuplet532>> Multiplets(boca::Event const& event, PreCuts const& pre_cuts, TMVA::Reader const& reader) override;

    std::string Name() const override;

    latex::String LatexName() const override;

private:

    std::vector<EventMultiplet<Decuplet532>> Events(boca::Event const& event, std::function<EventMultiplet<Decuplet532>(boca::EventMultiplet<Decuplet532> &)> const& function);

    Reader<SignatureSingleHadronic> signature_reader_;

    Reader<boca::tagger::Global> global_reader_;

};

}

}
