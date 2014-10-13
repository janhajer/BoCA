# ifndef HJetDiscriminator_hh
# define HJetDiscriminator_hh

# include "HAnalysis.hh"
# include "HEventDelphes.hh"

#include "HBranchDiscriminator.hh"



class HDiscriminatorJetTag : public HJetTag
{

    HBranchStruct GetBranchId(const int, HBranchStruct) const;

    const set<int> MotherParticle = {TopId, CpvHiggsId};

    const set<int> InitialState = {GluonId, UpId, DownId, StrangeId, CharmId, BottomId, UpDown0Id, UpDown1Id, UpUp1Id, DownDown1Id};

    const set<int> ChargeParticle = {UpId, DownId, StrangeId, CharmId, BottomId, TopId, PionId, RhoMesonId, ElectronId, MuonId, TauLeptonId, BMesonId, KMesonId, BMesonId, BMesonSId, ProtonId, DeltaBaryonId, KMesonSId, WId, DeltaBaryon2Id, DMesonId, DMesonS2Id, DMesonSId};

    const set<int> NeutralParticle = {DMeson0Id, KMeson0Id, KMeson0SId, NeutronId, BMeson0Id, BMeson0SId, DMesonS0Id, BMesonS0Id, BMesonSS0Id};

    string ClassName() const {

        return ("HDiscriminatorJetTag");

    };


};

/**
 * @brief Class defining the Disciminator Analysis
 *
 */
class HJetDiscriminator : public HAnalysis
{

public:

    /**
     * @brief Constructor
     *
     */
    HJetDiscriminator();

    /**
     * @brief Constructor
     *
     */
    ~HJetDiscriminator();

    /**
     * @brief Branch to write Higgs info into
     *
     */
    ExRootTreeBranch *CandidateBranch;

    /**
     * @brief Branch to write Lepton info into
     *
     */
    ExRootTreeBranch *SmallCandidateBranch;

    /**
     * @brief Branch to write Constituent info into
     *
     */
    ExRootTreeBranch *ConstituentBranch;

private:

    /**
     * @brief Lepton calculations
     *
     * @param Event ...
     * @return std::vector< fastjet::PseudoJet, std::allocator< void > >
     */
    vector<PseudoJet> Leptons();

    HDiscriminatorJetTag *HDiscriminatorTag;

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
    bool Analysis();

    /**
     * @brief prepares the vector describing the input root files
     *
     * @return void
     */
    void SetFileVector();

    /**
     * @brief New Analysis
     *
     * @return void
     */
    void NewFile();

    void CloseFile();

    vector<string> GetStudyNameVector();

    virtual string ClassName() const {

        return ("HJetDiscriminator");

    };

};


#endif

