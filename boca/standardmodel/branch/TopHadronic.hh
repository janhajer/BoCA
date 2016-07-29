/**
 * Copyright (C) 2015-2016 Jan Hajer
 */
#pragma once

#include "boca/branch/Multi.hh"
#include "boca/standardmodel/branch/BottomBase.hh"
#include "boca/standardmodel/branch/LeptonVeto.hh"

namespace boca
{

namespace standardmodel
{

namespace branch {

/**
 *
 * @brief Top tagger root tree structure
 *
 */
class TopHadronic : public boca::branch::Multi, public BottomBase, public LeptonVeto
{
public:

    TopHadronic();

    float bottom_mass;
    float w_mass;

    template<typename Multiplet>
    void Fill(Multiplet const& multiplet) {
        Multi::Fill(multiplet);
        BottomBase::Fill(multiplet);
        LeptonVeto::Fill(multiplet);
        bottom_mass = multiplet.Singlet().Mass() / GeV;
        w_mass = multiplet.Doublet().Mass() / GeV;
    }

    Observables Variables();

    Observables Spectators();

private:

    ClassDef(TopHadronic, 1)

};

}

}

}