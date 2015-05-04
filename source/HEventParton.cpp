# include "HEventParton.hh"
# include "HParticleParton.hh"


hanalysis::hparton::HEvent::HEvent()
{

    Print(HNotification, "Constructor");

    Particles = new HParticle();

}

hanalysis::hparton::HEvent::~HEvent()
{

    Print(HNotification, "Destructor");

    delete Particles;

}

void hanalysis::hparton::HEvent::NewEvent(const hanalysis::ClonesArrays &ClonesArrays)
{

    Print(HInformation, "New Event");

    Particles->NewEvent(ClonesArrays);

}
