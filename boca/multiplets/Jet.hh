/**
 * Copyright (C) 2015-2016 Jan Hajer
 */
#pragma once

#include "boca/fastjet/PseudoJet.hh"
#include "boca/multiplets/JetInfo.hh"

namespace boca
{

/**
* @brief Jet
*/
class Jet : public PseudoJet
{

public:

    Jet();

    Jet(double x, double y, double z, double e);

    Jet(const boca::Momentum& x, const boca::Momentum& y, const boca::Momentum& z, const boca::Energy& e);

    Jet(fastjet::PseudoJet const& jet);

    Jet(fastjet::PseudoJet const& jet, JetInfo const& info);

    Jet(LorentzVector<Momentum> const& lorentz_vector);

    Jet(TLorentzVector const& lorentz_vector);

    Jet(TLorentzVector const& lorentz_vector, ::delphes::Jet const& jet);

    Jet(TLorentzVector const& lorentz_vector, Constituent const& constituent);

    Jet(TLorentzVector const& lorentz_vector, std::vector<Constituent> const& constituents);

    Jet(TLorentzVector const& lorentz_vector, int charge);

    Jet(exroot::Electron const& electron);

    Jet(exroot::GenJet const& gen_jet);

    Jet(exroot::Jet const& jet);

    Jet(exroot::Muon const& muon);

    Jet(exroot::Photon const& photon);

    Jet(exroot::Tau const& tau);

    Jet(double const momentum[4]);

    JetInfo const& Info() const;

    JetInfo& Info();

    void SetDelphesTags(::delphes::Jet const& delphes_jet);

    std::vector<Jet> Constituents() const;

    double Bdt() const;

    void SetInfo(JetInfo const& info = JetInfo());

protected:

    void ResetInfo(JetInfo const& info);

private:

};

using Lepton = Jet;

using Photon = Jet;

using MissingEt = Jet;

std::vector<Jet> JetVector(std::vector<fastjet::PseudoJet> const& pseudo_jets);

std::vector<fastjet::PseudoJet> PseudoJetVector(std::vector<Jet> const& jets);

Jet Join(std::vector<Jet> const& jets);

Jet Join(Jet const& jet_1, Jet const& jet_2);

Jet Join(Jet const& jet_1, Jet const& jet_2, Jet const& jet_3);

}
