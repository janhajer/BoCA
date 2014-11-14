# include "HMva.hh"

hmva::HMva::HMva()
{

    Print(HNotification, "HMva", "Constructor");

    CutMethodName = "Cut";

    BdtMethodName = "Bdt";

    Cut = "";

}

hmva::HMva::~HMva()
{

    Print(HNotification, "Destructor");

}


HObservable hmva::HMva::NewObservable(float *const Value, const std::string& Expression, const std::string& Title, const std::string& Unit, const std::string& Latex) const
{

    Print(HNotification, "New Observable", Title);

    const HObservable Observable(Value, Expression, Title, Unit, Latex);

    return Observable;

}


HObservable hmva::HMva::NewObservable(float *const Value, const std::string& Expression, const std::string& Title, const std::string& Unit) const
{

    Print(HNotification, "New Observable", Title);

    const HObservable Observable(Value, Expression, Title, Unit, "");

    return Observable;

}

HObservable hmva::HMva::NewObservable(float *const Value, const std::string& Expression, const std::string& Title) const
{

    Print(HInformation, "New Observable", Expression);

    const HObservable Observable = NewObservable(Value, Expression, Title, "", "");

    return Observable;

}

HObservable hmva::HMva::NewObservable(float *const Value, const std::string& Expression) const
{

    Print(HInformation, "New Observable", Expression);

    const HObservable Observable = NewObservable(Value, Expression, Expression, "", "");

    return Observable;

}

HObservable::HObservable(float *const NewValue, const std::string& NewExpression, const std::string& NewTitle, const std::string& NewUnit, const std::string& NewLatex)
{

//     Print(HNotification, "Constructor");

    Value = NewValue;

    Expression = NewExpression;

    Title = NewTitle;

    Unit = NewUnit;

    Latex = NewLatex;

}

HObservable::HObservable()
{


}





// HObservable hmva::HMva::NewObservable(float *const Value, const std::string& Title, const std::string& Unit) const
// {
//
//   Print(HNotification, "New Observable", Title);
//
//   const std::string Expression = CandidateBranchName + "." + Title;
//
//   const HObservable Observable(Value, Expression, Title, Unit, "");
//
//   return Observable;
//
// }
