# ifndef HAnalysisJetProperties_hh
# define HAnalysisJetProperties_hh

# include "HAnalysis.hh"
# include "HEventDelphes.hh"

/**
 * @brief Class defining the Disciminator Analysis
 *
 */
class HAnalysisJetProperties : public HAnalysis
{

public:

    /**
     * @brief Constructor
     *
     */
    HAnalysisJetProperties();
    
    /**
     * @brief Branch to write Higgs info into
     *
     */
    ExRootTreeBranch *JetPropertiesBranch;
    
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

    /**
     * @brief Lepton calculations
     * 
     * @param Event ...
     * @return std::vector< fastjet::PseudoJet, std::allocator< void > >
     */
    vector<PseudoJet> Leptons();

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
    
    virtual string ClassName() {
        
        return ("HAnalysisJetProperties");
        
    };

};


#endif

