#pragma once

#include "boca/branch/ThreeBody.hh"
#include "boca/branch/EventShapesBase.hh"

namespace toppartner
{

namespace branch {

class SignatureEffectiveBranch : public boca::branch::ThreeBody, public boca::branch::EventShapesBase
{
public:
    SignatureEffectiveBranch();
    template<typename Multiplet>
    void Fill(Multiplet const& multiplet) {
        ThreeBody::Fill(multiplet);
        EventShapesBase::Fill(multiplet);
    }
    boca::Observables Variables();
    boca::Observables Spectators();
private:
    ClassDef(SignatureEffectiveBranch, 1)
};

}

}
