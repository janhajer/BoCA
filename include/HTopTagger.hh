# ifndef HTopTagger_hh
# define HTopTagger_hh

# include "fastjet/JetDefinition.hh"
# include "fastjet/ClusterSequence.hh"

# include "HObject.hh"
# include "HEPTopTagger.hh"

/**
 * @brief prepares the data for the top tagger
 *
 */
class HTopTagger : public HObject
{

public:

    /**
     * @brief tagges the tops
     *
     * @param HadronVector ...
     * @return void
     */
    vector< PseudoJet > GetTops(vector< PseudoJet > EFlowJets);

    /**
     * @brief constructor
     *
     */
    HTopTagger();

    ~HTopTagger();

private:

    /**
     * @brief Calculates Vector of granulated Jets
     *
     * @param HadronVector Vector of Jet Candidates derived from EFlow data
     * @param CellEta Delta Eta of Cells
     * @param CellPhi Delta Phi of Cells
     * @param PtCutOff Pt Cutoff for cells
     * @return std::vector< fastjet::PseudoJet, std::allocator >
     */
    vector<PseudoJet> GetGranulatedJets(vector<PseudoJet> &HadronVector, const float &CellEta, const float &CellPhi, const float &PtCutOff);

    inline string ClassName() const {

        return "HTopTagger";

    };

};

#endif
