# ifndef HMvaDiscriminator_hh
# define HMvaDiscriminator_hh


# include "Tagger.hh"
# include "HBranchDiscriminator.hh"

/**
 *
 * @brief Prepares multivariant analysis
 *
 */
class hcpvhiggs::HMva : public analysis::Tagger
{

public:

    /**
    * @brief Constructor
    *
    */
    HMva();

    /**
    * @brief Destructor
    *
    */
    ~HMva();

//     ReaderStruct CutLoop(const exroot::TreeReader * const, ReaderStruct&);

    void ApplyBdt(const exroot::TreeReader * const, const std::string, const TFile * const, const TMVA::Reader &);

    float GetBdt(TObject *, const TMVA::Reader &){ return 0;};

protected:


private:

      HCandidateBranch *Candidate;

    void DefineVariables();

    virtual inline std::string ClassName() const {
        return "HiggsCPV: HMva";
    };


};

# endif
