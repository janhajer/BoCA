# ifndef HMvaHiggs_hh
# define HMvaHiggs_hh


# include "Tagger.hh"
# include "HBranchHiggsCpv.hh"

/**
 *
 * @brief Prepares multivariant analysis
 *
 */
class hhiggscpv::HMvaHiggs : public analysis::Tagger
{

public:

    /**
    * @brief Constructor
    *
    */
    HMvaHiggs();

    /**
    * @brief Destructor
    *
    */
    ~HMvaHiggs();

//     ReaderStruct CutLoop(const exroot::TreeReader * const, ReaderStruct& ReaderStruct){ return ReaderStruct;};

    void ApplyBdt(const exroot::TreeReader * const, const std::string, const TFile * const, const TMVA::Reader &){};


    float GetBdt(TObject *Branch, const TMVA::Reader &Reader);

protected:


private:

  analysis::HiggsBranch *Higgs;

      void DefineVariables();

      virtual inline std::string NameSpaceName() const {
        return "HiggsCPV";
      };

    virtual inline std::string ClassName() const {
        return "HMva";
    };


};

# endif
