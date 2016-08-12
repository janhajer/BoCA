#include "include/tagger/EventPair.hh"
// #define DEBUGGING
#include "boca/generic/DEBUG_MACROS.hh"

namespace toppartner
{

namespace tagger
{

int EventPair::Train(boca::Event const& event, PreCuts const&, Tag tag)
{
    INFO0;
    return SaveEntries(Events(event, [&](boca::EventMultiplet<Decuplet55>& multiplet_event) {
        multiplet_event.SetTag(tag);
        return multiplet_event;
    }), tag);
}

std::vector<EventMultiplet<Decuplet55>> EventPair::Multiplets(boca::Event const& event, boca::PreCuts const&, TMVA::Reader const& reader)
{
    INFO0;
    return ReduceResult(Events(event, [&](boca::EventMultiplet<Decuplet55>& multiplet_event) {
        multiplet_event.SetBdt(Bdt(multiplet_event, reader));
        return multiplet_event;
    }), 1);
}

std::vector<EventMultiplet<Decuplet55>> EventPair::Events(boca::Event const& event, std::function<EventMultiplet<Decuplet55>(boca::EventMultiplet<Decuplet55> &)> const& function)
{
    INFO0;
    auto global_observables = global_reader_.Multiplets(event).front();
    std::vector<EventMultiplet<Decuplet55>> events;
    for (auto const & decuplet : signature_reader_.Multiplets(event)) {
        EventMultiplet<Decuplet55> multiplet_event(decuplet, global_observables);
        events.emplace_back(function(multiplet_event));
    }
    return events;
}

std::string EventPair::Name() const
{
    return "EventPair";
}

latex::String EventPair::LatexName() const
{
    return {"T_{h} T_{l}", true};
}

}

}
