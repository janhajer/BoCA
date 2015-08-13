#include "TopPartnerPairTagger.hh"
#include "Debug.hh"

namespace analysis
{

namespace toppartner
{

TopPartnerPairTagger::TopPartnerPairTagger()
{
    Info();
    DefineVariables();
}

int TopPartnerPairTagger::Train(Event const& event, PreCuts const&, Tag tag) const
{
    Info();
    std::vector<Decuplet55> decuplets = pairs(top_partner_hadronic_reader_.Multiplets(event), top_partner_leptonic_reader_.Multiplets(event), [&](Quintet const & quintet_1, Quintet const & quintet_2) {
        Decuplet55 decuplet(quintet_1, quintet_2);
        if (decuplet.Overlap()) throw "overlap";
        decuplet.SetTag(tag);
        return decuplet;
    });
    return SaveEntries(decuplets, 1);
}

std::vector<Decuplet55> TopPartnerPairTagger::Multiplets(Event const& event, analysis::PreCuts const&, TMVA::Reader const& reader) const
{
    Info();
    return ReduceResult(pairs(top_partner_hadronic_reader_.Multiplets(event), top_partner_leptonic_reader_.Multiplets(event), [&](Quintet const & quintet_1, Quintet const & quintet_2) {
        Decuplet55 decuplet(quintet_1, quintet_2);
        if (decuplet.Overlap()) throw "overlap";
        decuplet.SetBdt(Bdt(decuplet, reader));
        return decuplet;
    }));
}

}

}
