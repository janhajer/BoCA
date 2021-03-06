#pragma once

#include "standardmodel/analysis/StandardModel.hh"
#include "standardmodel/tagger/ZHadronic.hh"

namespace standardmodel
{

namespace analysis
{

/**
 *
 * @brief Z tagger ananlysis
 *
 * @author Jan Hajer
 *
 */
template<typename Tagger_>
class Z : public StandardModel<Tagger_>
{

public:


    Z() {
        this->PreCuts().PtLowerCut().Set(Id::Z, this->LowerPtCut());
        this->PreCuts().PtUpperCut().Set(Id::Z, this->UpperPtCut());
//         this->PreCuts().MassUpperCut().Set(Id::Z, 200_GeV);
        this->PreCuts().TrackerMaxEta().Set(Id::Z, Settings::TrackerEtaMax());
    }

private:

    std::string Name() const override {
        return boca::Name(this->Collider()) + "-" + boca::units::Name(this->LowerPtCut()) + "-large-2";
    }


    void SetFiles(Phase const& phase) override {
        switch (phase.Tag()) {
        case Tag::signal :
            if (!this->template TaggerIs<tagger::Bottom>()) this->AddSignal(Process::zz);
            if (this->template TaggerIs<tagger::Bottom>()) this->AddSignal(Process::bb);
            if (this->template TaggerIs<tagger::Bottom>()) this->AddSignal(Process::tt_had);
            if (this->template TaggerIs<tagger::Bottom>()) this->AddSignal(Process::tt_lep);
            if (this->template TaggerIs<tagger::Bottom>()) this->AddSignal(Process::hh);
            break;
        case Tag::background :
            if(phase.Stage() == Stage::reader)
          this->AddBackground(Process::ww);
            this->AddBackground(Process::qq);
            this->AddBackground(Process::gg);
            this->AddBackground(Process::cc);
            if (!this->template TaggerIs<tagger::Bottom>()) this->AddBackground(Process::tt_had);
            if (!this->template TaggerIs<tagger::Bottom>()) this->AddBackground(Process::hh_bb);
            if (!this->template TaggerIs<tagger::Bottom>()) this->AddBackground(Process::bb);
            if (!this->template TaggerIs<tagger::Bottom>()) this->AddBackground(Process::tt_lep);
            break;
        }
    }

    bool PassPreCut(boca::Event const&) const override {
        return true;
//        std::vector<Jet> particles = SortedByPt(event.GenParticles());
//         if ((particles.at(0).Pt() > this->LowerQuarkCut() && particles.at(0).Pt() < this->UpperQuarkCut()) && (particles.at(1).Pt() > this->LowerQuarkCut() &&  particles.at(1).Pt() < this->UpperQuarkCut())) return true;
//         return false;
    }

};

}

}
