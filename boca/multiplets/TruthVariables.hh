/**
 * Copyright (C) 2015-2016 Jan Hajer
 */
#pragma once

#include "boca/multiplets/Identification.hh"
#include "boca/multiplets/Particle.hh"
#include "boca/physics/Id.hh"
#include "boca/physics/Units.hh"

namespace boca
{

/**
 * @brief Variables for truth level analyses
 *
 */
class TruthVariables : public Identification
{

public:

    Momentum TopPt() const {
        return top_.Pt();
    }

    Momentum BottomPt() const {
        return bottom_.Pt();
    }

    Momentum WPt() const {
        return W_.Pt();
    }

    Momentum Quark1Pt() const {
        return quark_1_.Pt();
    }

    Momentum Quark2Pt() const {
        return quark_2_.Pt();
    }

    Angle DeltaRTop() const {
        return W_.DeltaRTo(bottom_)  ;
    }

    double RhoTop() const {
        return 2_rad * MassOf(Id::top) / DeltaRTop() / TopPt();
    }

    double RhoW() const {
        return 2_rad * MassOf(Id::W) / DeltaRW() / WPt();
    }

    Angle DeltaRW() const {
        return quark_1_.DeltaRTo(quark_2_)  ;
    }

    void SetTop(Particle const& top) {
        top_ = top;
    }

    void SetW(Particle const& W) {
        W_ = W;
    }

    void SetBottom(Particle const& bottom) {
        bottom_ = bottom;
    }

    void SetQuark1(Particle const& quark_1) {
        quark_1_ = quark_1;
    }

    void SetQuark2(Particle const& quark_2) {
        quark_2_ = quark_2;
    }

private:

    Particle top_;
    Particle W_;
    Particle bottom_;
    Particle quark_1_;
    Particle quark_2_;

};


}


