#include "include/branch/HeavyHiggsTau.hh"
#include "boca/OBSERVABLE.hh"

namespace heavyhiggs
{

namespace branch
{

HeavyHiggsTau::HeavyHiggsTau()
{
    NeutrinoPt = InitialValue();
    LeptonPt = InitialValue();
}

Observables HeavyHiggsTau::Variables()
{
    return Particle::Variables() + OBSERVABLE(LeptonPt) + OBSERVABLE(NeutrinoPt);
}

}

}
