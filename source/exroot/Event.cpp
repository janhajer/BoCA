
#include "boca/generic/Types.hh"
#include "boca/io/TreeReader.hh"
#include "boca/exroot/Classes.hh"
#include "boca/exroot/Event.hh"
#include "boca/generic/DEBUG_MACROS.hh"

namespace boca
{

namespace exroot
{

std::vector<Particle> Event::Particles(Status max_status) const
{
    INFO0;
    std::vector<Particle> particles;
    for (auto const & particle : TreeReader().Array<::exroot::LHEFParticle>(Branch::particle)) {
        if (particle.Status < to_int(max_status)) break;
        particles.emplace_back(Particle(particle, particle.PID));
    }
    return particles;
}

std::vector<Lepton> Event::Electrons() const
{
    INFO0;
    std::vector<Lepton> electrons;
    for (auto const & electron : TreeReader().Array<::exroot::Electron>(Branch::electron)) electrons.emplace_back(Lepton(electron));
    return electrons;
}

std::vector<Lepton> Event::Muons() const
{
    INFO0;
    std::vector<Lepton> muons;
    for (auto const & muon : TreeReader().Array<::exroot::Muon>(Branch::muon)) muons.emplace_back(Lepton(muon));
    return muons;
}

std::vector<Lepton> Event::Photons() const
{
    INFO0;
    std::vector<Lepton> photons;
    for (auto const & photon : TreeReader().Array<::exroot::Photon>(Branch::photon)) photons.emplace_back(Lepton(photon));
    return photons;
}

std::vector<Jet> Event::Jets() const
{
    INFO0;
    std::vector<Jet> jets;
    for (auto const & exroot_jet : TreeReader().Array<::exroot::Jet>(Branch::jet)) {
        Jet jet(exroot_jet);
        jet.Info().SetBTag(exroot_jet.BTag);
        jets.emplace_back(jet);
    }
    return jets;
}

}

}
