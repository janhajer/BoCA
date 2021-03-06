#include "boca/multiplets/Quintet.hh"
#include "boca/multiplets/Doublet.hh"

namespace toppartner
{

using namespace boca;

class Septet : public TwoBody<boca::Quintet, boca::Doublet>
{

public:

    using TwoBody<boca::Quintet, boca::Doublet>::TwoBody;

    boca::Quintet const& Quintet() const {
        return Multiplet1();
    }

    boca::Doublet const& Doublet() const {
        return Multiplet2();
    }

};

}
