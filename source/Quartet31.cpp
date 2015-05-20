# include "Quartet31.hh"

void analysis::Quartet31::Settriplet(const Triplet &triplet)
{
    triplet_ = triplet;
}

void analysis::Quartet31::SetSinglet(const fastjet::PseudoJet &singlet)
{
    singlet_ = singlet;
}

analysis::Quartet31::Quartet31()
{
    Print(kInformation, "Constructor");
}

analysis::Quartet31::Quartet31(const Triplet &triplet, const fastjet::PseudoJet &singlet)
{
    Print(kInformation, "Constructor");
    Settriplet(triplet);
    SetSinglet(singlet);
    if (singlet_.has_user_info<JetInfo>()) {
        SetBdt(triplet_.Bdt(), singlet_.user_info<JetInfo>().Bdt());
        SetTag(triplet_.Tag(), singlet_.user_info<JetInfo>().Tag());
    }
    SetFlag(triplet.Flag());
}

analysis::Triplet analysis::Quartet31::triplet() const
{
    return triplet_;
}

fastjet::PseudoJet analysis::Quartet31::singlet()const
{
    return singlet_;
}