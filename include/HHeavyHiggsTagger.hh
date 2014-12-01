# ifndef HHeavyHiggsTagger_hh
# define HHeavyHiggsTagger_hh

# include "HMva.hh"
# include "HBranch.hh"
# include "HEvent.hh"
# include "HJetTag.hh"
# include "HReader.hh"
# include "HLeptonicTopTagger.hh"
# include "HPairPair.hh"

/**
 * @brief calculation regarding leptons
 *
 */
class hdelphes::HHeavyHiggsTagger : public hmva::HMva
{

public:

    HHeavyHiggsTagger(HBottomTagger *NewBottomTagger, HLeptonicTopTagger *NewTopTagger);

    ~HHeavyHiggsTagger();

    std::vector<HMvaHeavyHiggsBranch *> GetBranches(hanalysis::HEvent *const Event, const hanalysis::HObject::HState State);
    
    void FillBranch(const hanalysis::HPairPair &HeavyHiggs);
    
    hdelphes::HBottomTagger *BottomTagger;
    hdelphes::HLeptonicTopTagger *TopTagger;
    
private:

    hmva::HReader *BottomReader;
    
    hmva::HReader *TopReader;

    HMvaHeavyHiggsBranch *Branch;

    hanalysis::HJetTag *JetTag;

    void DefineVariables();

    void FillBranch(HMvaHeavyHiggsBranch *HeavyHiggsRoot,const hanalysis::HPairPair &HeavyHiggs);

    virtual inline std::string NameSpaceName() const {
        return "HDelphes";
    };

    virtual inline std::string ClassName() const {
        return "HHeavyHiggsTagger";
    };

};

#endif