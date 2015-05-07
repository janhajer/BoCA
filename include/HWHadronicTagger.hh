# ifndef HHadronicWTagger_hh
# define HHadronicWTagger_hh

# include "BottomTagger.hh"
# include "HDoublet.hh"
# include "Reader.hh"

/**
 * @brief W BDT tagger
 *
 */
class hanalysis::HWHadronicTagger : public Tagger
{

public:

    HWHadronicTagger();

    void SetTagger(const BottomTagger &NewBottomTagger);

    std::vector< HWBranch > GetBranches(hanalysis::HEvent &Event, const hanalysis::HObject::Tag Tag);

//     std::vector<HParticleBranch> GetConstituentBranches();

    std::vector<HDoublet> GetBdt(const HJets &Jets, const hanalysis::Reader &WReader, hanalysis::BottomTagger &bottom_tagger, hanalysis::Reader &BottomReader);

    std::vector<HDoublet> GetBdt(const HJets &Jets, const hanalysis::Reader &WReader);
    std::vector<HDoublet> GetPairBdt(const HJets &Jets, const hanalysis::Reader &WReader);
    std::vector<HDoublet> GetSingletBdt(const HJets &Jets, const hanalysis::Reader &WReader);

    HDoublet GetBdt(HDoublet &Doublet, const hanalysis::Reader &WReader);

    BottomTagger bottom_tagger_;

    Reader bottom_reader_;

    HWBranch GetBranch(const HDoublet &Doublet) const;

    int GetWHadId(hanalysis::HEvent &Event) {
      return GetWHadId(GetWDaughters(Event));
    };

protected:

    virtual inline std::string ClassName() const {
        return "HHadronicWTagger";
    }

private:

    HJets GetWDaughters(hanalysis::HEvent &Event);

    int GetWHadId(const HJets &Jets);

    void DefineVariables();

//     HParticleBranch GetBranch(const HKinematics &Vector);

//     hanalysis::HObject::HTag GetTag(const HDoublet &Doublet);

//     hanalysis::HObject::HTag GetTag(const fastjet::PseudoJet &Singlet);

    HWBranch Branch;
    HJetTag JetTag;
    float WMassWindow;
};

#endif
