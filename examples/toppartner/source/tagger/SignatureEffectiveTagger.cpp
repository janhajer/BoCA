#include "include/tagger/SignatureEffectiveTagger.hh"
#include "boca/generic/Exception.hh"
// #define DEBUGGING
#include "boca/generic/DEBUG.hh"

namespace toppartner
{

int SignatureEffectiveTagger::Train(boca::Event const& event, PreCuts const&, Tag tag)
{
    INFO0;
    return SaveEntries(Quattuordecuplets(event, [&](Quattuordecuplet554 & quattuordecuplet) {
        quattuordecuplet.SetTag(tag);
        return quattuordecuplet;
    }), tag);
}

std::vector<Quattuordecuplet554> SignatureEffectiveTagger::Multiplets(boca::Event const& event, boca::PreCuts const&, TMVA::Reader const& reader)
{
    INFO0;
    return ReduceResult(Quattuordecuplets(event, [&](Quattuordecuplet554 & quattuordecuplet) {
        quattuordecuplet.SetBdt(Bdt(quattuordecuplet, reader));
        return quattuordecuplet;
    }), 1);
}

std::vector<Quattuordecuplet554> SignatureEffectiveTagger::Quattuordecuplets(boca::Event const& event, std::function< Quattuordecuplet554(Quattuordecuplet554&)> const& function)
{
    INFO0;
    auto hadronic = top_partner_hadronic_reader_.Multiplets(event, 8);
    auto leptonic = top_partner_leptonic_reader_.Multiplets(event, 8);
    auto higgs_pairs = higgs_pair_reader_.Multiplets(event, 8);
    auto signatures = Triples(hadronic, leptonic, higgs_pairs, [&](Quintet const & quintet_1, Quintet const & quintet_2) {
        Decuplet55 decuplet(quintet_1, quintet_2);
        if (decuplet.Overlap()) throw Overlap();
        return decuplet;
    }, [&](Decuplet55 const & decuplet, Quartet22 const & quartet) {
        Quattuordecuplet554 quattuordecuplet;
        quattuordecuplet.SetMultiplets12(decuplet, quartet);
        if (quattuordecuplet.Overlap()) throw Overlap();
        return function(quattuordecuplet);
    });
//     static int sig = 0;
//     static int eve = 0;
//     ++eve;
//     if (!signatures.empty()) ++sig;
//     double fraction = static_cast<double>(sig) / eve;
//     ERROR(signatures.size(), hadronic.size(), leptonic.size(), higgs_pairs.size(), fraction);
    return signatures;
}

std::string SignatureEffectiveTagger::Name() const
{
    return "Signature";
}

latex::String SignatureEffectiveTagger::LatexName() const
{
    return "T_{h} T_{l} h h";
}

}


