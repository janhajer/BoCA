# ifndef HMvaDiscriminator_hh
# define HMvaDiscriminator_hh


# include "HMva.hh"
# include "HBranchDiscriminator.hh"

/**
 * \class HMvaDiscriminator
 *
 * \ingroup HDiscriminator
 *
 * @brief Prepares multivariant analysis
 *
 */
class HMvaDiscriminator : public HMva
{

public:

    /**
    * @brief Constructor
    *
    */
    HMvaDiscriminator();

    /**
    * @brief Destructor
    *
    */
    ~HMvaDiscriminator();

    HReaderStruct CutLoop(const ExRootTreeReader * const, HReaderStruct&);

    void ApplyBdt(const ExRootTreeReader * const, const string, const TFile * const, TMVA::Reader *);

protected:


private:

      HCandidateBranch *Candidate;

    void DefineVariables();

    virtual string ClassName() const {
        return ("HMvaDiscriminator");
    };


};

# endif
