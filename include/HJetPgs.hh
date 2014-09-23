# ifndef HJetPgs_hh
# define HJetPgs_hh

# include "HJet.hh"
# include "HFourVector.hh"


/**
 * @brief stores all the information about the event topology
 *
 */
class HJetPgs : public HJet, public HFourVectorExRoot
{

public:
    
    /**
     * @brief constructor
     *
     */
    HJetPgs();
    
    /**
     * @brief Destructor
     *
     */
    ~HJetPgs();
    
    /**
     * @brief AnalyseJet calls AnalyseEFlow
     *
     * @return void
     */
    bool GetJets();
    
private:
    
    virtual TString ClassName() {
        return ("HJetPgs");
    };
    
};

#endif