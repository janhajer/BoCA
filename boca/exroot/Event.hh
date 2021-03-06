/**
 * Copyright (C) 2015-2016 Jan Hajer
 */
#pragma once

#include "boca/Event.hh"

namespace boca
{

/**
 * @brief ExRoot
 *
 */
namespace exroot
{

/**
 * @brief pgs and parton events
 *
 */
class Event : public boca::Event
{

public:

    using boca::Event::Event;

    std::vector<Lepton> Electrons() const override;

    std::vector<Lepton> Muons() const override;

    std::vector<Photon> Photons() const override;

    std::vector<Jet> Jets() const override;

private:

    std::vector<Particle> Particles(Status max_status) const;


};

}

}
