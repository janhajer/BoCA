# ifndef HAnalysisDiscriminator_hh
# define HAnalysisDiscriminator_hh

# include "HAnalysis.hh"
# include "HEventDelphes.hh"
# include "HBranchDiscriminator.hh"
# include "HSubStructure.hh"

/**
 * \class HDiscriminatorJetTag
 *
 * @brief HJetTag subclass for HDiscriminator
 *
 */
class Discriminator::HJetTag : public Analysis::HJetTag
{

    int GetBranchId(const int ParticleId, int BranchId);

    const set<int> HeavyParticles = {TopId, CpvHiggsId, HiggsId};

    virtual inline string ClassName() const {

        return "Discriminator: HJetTag";

    };

};

/**
 * \class HAnalysisDiscriminator
 *
 * @brief HAnalysis subclass defining the Discriminator Analysis
 *
 * \author Jan Hajer
 *
 */
class Discriminator::HAnalysis : public Analysis::HAnalysis
{

public:

    /**
     * @brief Constructor
     *
     */
    HAnalysis();

    /**
     * @brief Constructor
     *
     */
    ~HAnalysis();

    /**
     * @brief Branch to write Higgs info into
     *
     */
    ExRootTreeBranch *CandidateBranch;

    /**
     * @brief Branch to write Lepton info into
     *
     */
    ExRootTreeBranch *LeptonBranch;

    /**
     * @brief Branch to write Constituent info into
     *
     */
    ExRootTreeBranch *ConstituentBranch;

private:

    inline int GetEventNumberMax() const {

        return 10000;

    };

    inline string GetProjectName()const {

        return "Discriminator";

    };

    Discriminator::HJetTag * JetTag;

    Analysis::HSubStructure * SubStructure;

    /**
     * @brief Lepton calculations
     *
     * @param Event ...
     * @return std::vector< fastjet::PseudoJet, std::allocator< void > >
     */
    vector<PseudoJet> GetLeptonJets(Analysis::HEvent *const Event);

    /**
     * @brief Lepton event counter
     *
     */
    int LeptonEventCounter;

    /**
     * @brief Main Analysis function
     *
     * @return void
     */
    bool Analysis(Analysis::HEvent *const Event, const string StudyName);

    /**
     * @brief prepares the vector describing the input root files
     *
     * @return void
     */
    std::vector<Analysis::HFile * > GetFiles(const string StudyName) const;

    /**
     * @brief New Analysis
     *
     * @return void
     */
    void NewBranches(ExRootTreeWriter *TreeWriter);

    inline vector<string> GetStudyNames() const;

    virtual inline string ClassName() const {

        return "Discriminator: HAnalysis";

    };

};

#endif

