/**
 * Copyright (C) 2015-2016 Jan Hajer
 */
#pragma once

#include "fastjet/JetDefinition.hh"

namespace boca
{

/**
  * @ingroup FastJet
  * @brief Recombine two fastjet::Jet with attached fastjet::UserInfo
  *
  */
class InfoRecombiner : public fastjet::JetDefinition::DefaultRecombiner
{

public:

  InfoRecombiner(fastjet::RecombinationScheme recombination_scheme = fastjet::E_scheme);

  std::string description() const override;

  /**
   * @brief recombine jet_1 and jet_2 and store result in jet
   *
   */
  void recombine(fastjet::PseudoJet const& jet_1, fastjet::PseudoJet const& jet_2, fastjet::PseudoJet& jet) const override;

private:

};

}
