# ifndef HChargedJetPairTagger_hh
# define HChargedJetPairTagger_hh

# include "HBottomTagger.hh"
# include "HQuartet31.hh"
# include "HTopSemiTagger.hh"
# include "HTopHadronicTagger.hh"

/**
 * @brief JetPair BDT tagger
 *
 */
class hanalysis::HChargedJetPairTagger : public HMva
{

public:

    HChargedJetPairTagger();

    ~HChargedJetPairTagger();

    void SetTagger(const HBottomTagger &NewBottomTagger, const HWSemiTagger &NewWSemiTagger, const HWTagger &NewWTagger, const HTopSemiTagger &NewTopSemiTagger, const HTopHadronicTagger &NewTopHadronicTagger);

    std::vector< HChargedJetPairBranch> GetBranches(HEvent *const Event, const HObject::HTag Tag);

    std::vector< HQuartet31 > GetBdt(const std::vector< hanalysis::HTriplet > &Triplets, const HJets &Jets, const hanalysis::HReader &JetPairReader);

    HChargedJetPairBranch GetBranch(const hanalysis::HQuartet31 &Quartet) const;

protected:

    virtual inline std::string ClassName() const {
        return "HChargedJetPairTagger";
    }

private:

    void DefineVariables();

    hanalysis::HObject::HTag GetTag(const hanalysis::HQuartet31 &Quartet);

    HBottomTagger BottomTagger;
    HWTagger WTagger;
    HWSemiTagger WSemiTagger;
    HTopHadronicTagger TopHadronicTagger;
    HTopSemiTagger TopSemiTagger;

    HReader BottomReader;
    HReader WReader;
    HReader WSemiReader;
    HReader TopHadronicReader;
    HReader TopSemiReader;

    HChargedJetPairBranch Branch;

    HJetTag JetTag;

};

#endif
