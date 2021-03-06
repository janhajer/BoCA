#include "heavyhiggs/analysis/NeutralFourTop.hh"
#include "heavyhiggs/tagger/Cut.hh"

template<typename Tagger_>
void Run(boca::Output output)
{
    heavyhiggs::analysis::NeutralFourTop<Tagger_> analysis;
    analysis.Run(output);
}

int main()
{
    Run<standardmodel::tagger::Bottom>(boca::Output::fast);
    Run<heavyhiggs::tagger::Cut>(boca::Output::cut);
}
