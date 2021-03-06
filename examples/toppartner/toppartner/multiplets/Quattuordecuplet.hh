#pragma once

#include "boca/multiplets/Quartet.hh"

#include "toppartner/multiplets/Decuplet.hh"

namespace toppartner {

class Quattuordecuplet104 : public TwoBody<toppartner::Decuplet55, Quartet22> {

public:

    using TwoBody<toppartner::Decuplet55, Quartet22>::TwoBody;

    toppartner::Decuplet55 Decuplet() const
    {
        return Multiplet1();
    }

    Quartet22 Quartet() const
    {
        return Multiplet2();
    }

};

class Quattuordecuplet554 : public ThreeBody<Quintet, Quintet, Quartet22> {

public:

  using ThreeBody<Quintet, Quintet, Quartet22>::ThreeBody;

  Quintet Quintet_1() const
  {
    return Multiplet1();
  }

  Quintet Quintet_2() const
  {
    return Multiplet2();
  }

  Quartet22 Quartet() const
  {
    return Multiplet3();
  }

};

}
