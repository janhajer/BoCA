/**
 * Copyright (C) 2015-2016 Jan Hajer
 */;
#include "boca/OBSERVABLE_MACROS.hh"

#include "tthh/branch/Global.hh"

namespace tthh {

namespace branch{

Global::Global()
{
    jet_number = static_cast<int>(InitialValue());
    bottom_number = static_cast<int>(InitialValue());
    missing_et = InitialValue();
    scalar_ht = InitialValue();
    jet_1_pt = InitialValue();
    jet_2_pt = InitialValue();
    jet_3_pt = InitialValue();
    jet_4_pt = InitialValue();
    photon_1_pt = InitialValue();
    photon_2_pt = InitialValue();
    photon12_mass = InitialValue();
}

boca::Observables Global::Variables()
{
    return boca::branch::Bdt::Variables() + OBSERVABLE(bottom_number, "#b") + OBSERVABLE(jet_number, "#j") + OBSERVABLE(missing_et, "E_{T}^{miss}") + OBSERVABLE(scalar_ht, "H_{T}^{scalar}") + OBSERVABLE(jet_1_pt, "p_{T}(j_{1})") + OBSERVABLE(jet_2_pt, "p_{T}(j_{2})") + OBSERVABLE(jet_3_pt, "p_{T}(j_{3})") + OBSERVABLE(jet_4_pt, "p_{T}(j_{4})") + OBSERVABLE(photon_1_pt, "p_{T}(#gamma_{1})") + OBSERVABLE(photon_2_pt, "p_{T}(#gamma_{2})") + OBSERVABLE(photon12_mass, "m_{#gamma#gamma}");
}

boca::Observables Global::Spectators()
{
    return boca::branch::Bdt::Spectators();
}

}

}
