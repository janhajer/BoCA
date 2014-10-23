# ifndef HDiscriminator_hh
# define HDiscriminator_hh

# include "HReconstruction.hh"
# include "HJetInfo.hh"

using std::sort;

/**
 * @brief Jet Discriminator
 *
 *
 */
class HDiscriminator : public HReconstruction
{

public:

    /**
     * @brief Constructor
     *
     */
    HDiscriminator();

    /**
     * @brief Destructor
     *
     */
    ~HDiscriminator();

    /**
     * @brief Get Fat Higgs Jets
     *
     * @param  EFlow Jet Vector
     * @return Candidates
     */
    vector<PseudoJet> GetCandidateJets(const vector<PseudoJet> &, const float);
    
    vector<PseudoJet> GetCandidateJetsForced(const vector<PseudoJet> &, const float);
    
    static bool JetIsBad(const PseudoJet &Jet);

private:

    string ClassName() const {

        return ("HDiscriminator");

    };

};

# endif
