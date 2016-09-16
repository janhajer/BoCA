/**
 * Copyright (C) 2015-2016 Jan Hajer
 */
#pragma once

#include "fastjet/PseudoJet.hh"
#include "boca/math/LorentzVector.hh"

class TLorentzVector;

namespace boca
{

template<typename Value_>
class LorentzVector;

/**
* @ingroup FastJet
* @brief Wrapper for fastjet::PseudoJet adding BoCA related functions
*
*/
class PseudoJet : public fastjet::PseudoJet
{
public:
    /**
    * @name Constructors
    */
    //@{
    using fastjet::PseudoJet::PseudoJet;
    /**
    * @brief Default Constructor
    */
    PseudoJet();
    /**
    * @brief Constructor accepting root TLorentzVector
    */
    PseudoJet(TLorentzVector const& vector);
    /**
    * @brief Constructor accepting momentum lorentz vector
    */
    PseudoJet(LorentzVector<Momentum> const& vector);
    //@}
    /**
    * @brief Rescale the jet momentum
    *
    * doesnt change the jet energy
    */
    void ScaleMomentum(double factor);
    /**
    * @name Accesors for vectors
    */
    //@{
    /**
    * @brief Momentum lorentz vector
    */
    LorentzVector< Momentum > Vector() const;
    /**
    * @brief Momentum three vector
    */
    boca::Vector3<Momentum> Vector3() const;
    //@}
    /**
    * @name Accesor for UserInfo
    */
    //@{
    virtual UserInfoBase const& Info() const;

    virtual UserInfoBase& Info();
    //@}
    /**
    * @name Energy related accessors
    */
    //@{
    /**
    * @brief Transverse Momentum
    */
    Momentum Pt() const;
    /**
    * @brief Energy
    */
    boca::Energy Energy() const;
    /**
    * @brief Mass
    */
    boca::Mass Mass() const;
    /**
    * @brief Mass square
    */
    boca::MassSquare MassSquare() const;
    /**
    * @brief X component
    */
    Momentum Px() const;
    /**
    * @brief Y component
    */
    Momentum Py() const;
    /**
    * @brief Z component
    */
    Momentum Pz() const;
    /**
    * @brief squared 3-vector modulus \f$p_x^2+p_y^2+p_z^2\f$
    */
    MomentumSquare ModP2() const;
    /**
    * @brief the 3-vector modulus \f$\sqrt{p_x^2+p_y^2+p_z^2}\f$
    */
    Momentum ModP() const;
    //@}
    /**
    * @name Angle related accessors
    */
    //@{
    /**
    * @brief Rapidity \f$y\f$
    */
    Angle Rap() const;
    /**
    * @brief Azimuth \f$\phi\f$ constrained to \f$[-\pi,\pi]\f$
    */
    Angle Phi() const;
    /**
    * @brief \f$\Delta \phi\f$ to a jet constrained to \f$[-\pi,\pi]\f$
    */
    Angle DeltaPhiTo(PseudoJet const& jet) const;
    /**
    * @brief \f$\Delta y\f$ to a jet
    */
    Angle DeltaRapTo(PseudoJet const& jet) const;
    /**
    * @brief \f$\Delta R\f$ to a jet
    */
    Angle DeltaRTo(PseudoJet const& jet) const;
    /**
    * @brief Angular distance to a jet
    */
    Vector2<Angle> DeltaTo(PseudoJet const& jet) const;
    /**
    * @brief Vector of rapidity \f$y\f$ and azimuth \f$\phi\f$
    * @param wrap_phi use the given \f$[-\pi,\pi]\f$ or wrap it by \f$2 \pi\f$
    */
    Vector2<Angle> Angles(bool wrap_phi = false) const;
    /**
    * @brief Vector of rapidity \f$y\f$ and azimuth \f$\phi\f$ with minimal distance to jet
    */
    Vector2<Angle> AnglesMinTo(PseudoJet const& jet) const;
    //@}

    bool operator<(PseudoJet const& jet) const {
        return Pt() < jet.Pt();
    }

};

}
