# ifndef HAnalysisTopTagger_hh
# define HAnalysisTopTagger_hh

# include "HAnalysis.hh"
# include "HFileDelphes.hh"
# include "HEventDelphes.hh"
# include "HBranchTopTagger.hh"
# include "HSubStructure.hh"

/**
 *
 * @brief HJetTag subclass for HDiscriminator
 *
 */
class htoptagger::HJetTag : public hanalysis::HJetTag
{

    int GetBranchId(const int ParticleId, int BranchId);

    const std::set<int> HeavyParticles = {TopId, CpvHiggsId};

    virtual inline std::string ClassName() const {

        return "HDiscriminatorJetTag";

    };

};

/**
 *
 * @brief HAnalysis ubclass defining the Disciminator Analysis
 *
 * \author Jan Hajer
 *
 */
class htoptagger::HAnalysis : public hanalysis::HAnalysis
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

    inline int EventNumberMax() const {

        return 10000;

    };

    inline std::string ProjectName()const {

        return "TopTagger";

    };

    hanalysis::HJetTag JetTag;

    hanalysis::HSubStructure *SubStructure;

    /**
     * @brief Lepton calculations
     *
     * @param Event ...
     * @return std::vector< fastjet::PseudoJet, std::allocator< void > >
     */
    HJets GetLeptonJets(hanalysis::HEvent &Event);

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
    bool Analysis(hanalysis::HEvent &Event, const std::string &Name);

    /**
     * @brief prepares the std::vector describing the input root files
     *
     * @return void
     */
    std::vector< hanalysis::HFile * > GetFiles(const std::string &Name);

    /**
     * @brief New Analysis
     *
     * @return void
     */
    void NewBranches(ExRootTreeWriter *NewTreeWriter);

    void CloseFile();

    inline HStrings GetStudyNames() const;

    virtual inline std::string ClassName() const {

        return "htoptagger: HAnalysis";

    };

};


#endif

