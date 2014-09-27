# ifndef HEventDelphes_hh
# define HEventDelphes_hh

# include "HEvent.hh"
# include "HParticleDelphes.hh"
# include "HLeptonDelphes.hh"
# include "HJetDelphes.hh"

/**
 * @brief stores all the information about the event topology
 *
 */
class HEventDelphes : public HEvent
{

public:

    /**
    * @brief constructor
    *
    */
    HEventDelphes();

    /**
     * @brief destructor
     *
     */
    ~HEventDelphes();
    
    void NewFile();
    
    void CloseFile();
    
    void NewEvent(HClonesArray *);

    vector<TLorentzVector>GetLeptons();

    void GetJets();
    
    void GetEFlow();
    
    void GetParticles();

    PseudoJet GetHiggs();
    
    vector<PseudoJet> GetHiggsTopCandidates();

    vector<PseudoJet> GetTops();
    
    void GetTaggedJets();

private:
    
    virtual TString ClassName() {
        return ("HEventDelphes");
    };
    
    bool HasEFlow;
    
    bool HasParticles;
    
    bool HasJets;

};

#endif

