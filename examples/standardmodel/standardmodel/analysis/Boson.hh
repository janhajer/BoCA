#pragma once

#include "standardmodel/analysis/StandardModel.hh"
#include "standardmodel/tagger/Bottom.hh"

namespace standardmodel
{

namespace analysis{

/**
 *
 * @brief Higgs tagger analysis
 *
 * @author Jan Hajer
 *
 */
template<typename Tagger_>
class Boson : public StandardModel<Tagger_>
{

public:

    Boson() {
        this->PreCuts().PtLowerCut().Set(MultiId::neutral_boson, this->LowerPtCut());
        this->PreCuts().PtUpperCut().Set(MultiId::neutral_boson, this->UpperPtCut());
//         this->PreCuts().PtLowerCut().Set(Id::bottom, this->LowerPtCut() / 5.);
//         this->PreCuts().PtUpperCut().Set(Id::bottom, this->UpperPtCut() / 5.);
        //         this->PreCuts().MassUpperCut().Set(MultiId::neutral_boson, 250_GeV);
        this->PreCuts().TrackerMaxEta().Set(MultiId::neutral_boson, Settings::TrackerEtaMax());
        this->PreCuts().TrackerMaxEta().Set(Id::bottom, Settings::TrackerEtaMax());
    }

private:

    std::string Name() const override {
        return  boca::Name(this->Collider()) + "-" + boca::units::Name(this->LowerPtCut()) + "-large";
    }

    void SetFiles(Tag tag, Stage stage) override {
        switch (tag) {
        case Tag::signal :
            this->NewFile(tag, Process::hh_bb);
            if (!this->template TaggerIs<tagger::Bottom>()) this->NewFile(tag, Process::zz);
//             if (!this->template TaggerIs<tagger::Bottom>()) this->NewFile(tag, Process::ww);
            if (this->template TaggerIs<tagger::Bottom>()) this->NewFile(tag, Process::bb);
            if (this->template TaggerIs<tagger::Bottom>()) this->NewFile(tag, Process::tt_had);
            if (this->template TaggerIs<tagger::Bottom>()) this->NewFile(tag, Process::tt_lep);
            break;
        case Tag::background :
            if (!this->template TaggerIs<tagger::Bottom>()) this->NewFile(tag, Process::tt_lep);
            if (!this->template TaggerIs<tagger::Bottom>()) this->NewFile(tag, Process::tt_had);
            if (!this->template TaggerIs<tagger::Bottom>()) this->NewFile(tag, Process::bb);
            this->NewFile(tag, Process::gg);
            this->NewFile(tag, Process::cc);
            this->NewFile(tag, Process::qq);
            if(stage == Stage::reader) this->NewFile(tag, Process::ww);
            break;
        }

    }

    bool PassPreCut(boca::Event const&) const override {
          return true;
//        std::vector<Jet> jets = SortedByPt(event.Jets());
//         if (jets.empty()) return false;
//         if (jets.front().Pt() < this->LowerPtCut()) return false;
//        std::vector<Jet> particles = SortedByPt(event.GenParticles());
//         if ((particles.at(0).Pt() > this->LowerQuarkCut() && particles.at(0).Pt() < this->UpperQuarkCut()) && (particles.at(1).Pt() > this->LowerQuarkCut() &&  particles.at(1).Pt() < this->UpperQuarkCut())) return true;
        return false;
    }

};

}

}
