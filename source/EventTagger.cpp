#include "boca/EventTagger.hh"
#include "boca/Event.hh"
#include "boca/generic/DEBUG_MACROS.hh"

namespace boca
{

int EventTagger::Train(boca::Event const& event, PreCuts const& , Tag tag)
{
    INFO0;
    EventMultiplet<Tagger_> event_multiplet(signature_reader.Multiplets(), global_reader.Multiplets());
    event_multiplet.SetTag(tag);
    return SaveEntries( {event_multiplet});
}

std::vector<GlobalObservables> EventTagger::Multiplets(Event const& event, PreCuts const& , TMVA::Reader const& reader)
{
    INFO0;
    EventMultiplet<Tagger_> event_multiplet(signature_reader.Multiplets(), global_reader.Multiplets());
    event_multiplet.SetBdt(Bdt(global_observables, reader));
    return {event_multiplet};
}

std::string EventTagger::Name() const
{
    return "Event";
}

}
