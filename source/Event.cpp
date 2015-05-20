# include "Event.hh"
# include "delphes/Partons.hh"
# include "delphes/Leptons.hh"
# include "delphes/Hadrons.hh"
# include "exroot/Leptons.hh"
# include "exroot/Hadrons.hh"
# include "exroot/Partons.hh"


analysis::Event::Event(const ClonesArrays::Source source)
{
    Print(kNotification, "Constructor");
    source_ = source;
    switch (source_) {
    case ClonesArrays::kDelphes :
        partons_ = new delphes::Partons();
        hadrons_ = new delphes::Hadrons();
        leptons_ = new delphes::Leptons();
        break;
    case ClonesArrays::kPgs :
        leptons_ = new exroot::Leptons();
        hadrons_ = new exroot::Hadrons();
        break;
    case ClonesArrays::kParton :
        partons_ = new exroot::Partons();
        break;
    }
}

analysis::Event::~Event()
{
    Print(kNotification, "Destructor");
    switch (source_) {
    case ClonesArrays::kDelphes :
        delete partons_;
        delete leptons_;
        delete hadrons_;
        break;
    case ClonesArrays::kPgs :
        delete leptons_;
        delete hadrons_;
        break;
    case ClonesArrays::kParton:
        delete partons_;
        break;
    }
}

void analysis::Event::NewEvent(const analysis::ClonesArrays &clones_arrays)
{
    Print(kInformation, "New event");
    switch (source_) {
    case ClonesArrays::kDelphes :
        partons_->NewEvent(clones_arrays);
        hadrons_->NewEvent(clones_arrays);
        leptons_->NewEvent(clones_arrays);
        break;
    case ClonesArrays::kPgs :
        hadrons_->NewEvent(clones_arrays);
        leptons_->NewEvent(clones_arrays);
        break;
    case ClonesArrays::kParton:
        partons_->NewEvent(clones_arrays);
        break;
    }
}