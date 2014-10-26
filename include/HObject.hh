# ifndef HObject_hh
# define HObject_hh

# include <iostream>
# include <iomanip>
# include <cmath>
# include <vector>

# include "TMath.h"
# include "TLorentzVector.h"

# include "fastjet/PseudoJet.hh"

using fastjet::PseudoJet;
using std::string;
using std::vector;

/**
 * @brief Main Analysis Library
 *
 */
namespace Analysis{

  class HObject;
  class HFourVector;
  class HClonesArray;
  class HParticle;
  class HLepton;
  class HTopTagger;
  class HReconstruction;
  class HHiggsTagger;
  class HDiscriminator;
  class HEvent;
  class HFile;
  class HFileFolder;
  class HAnalysis;

  class HJetTag;
  class HJetInfo;
  class HJet;
  class HPull;
  class HSubStructure;

  class HMva;
  class HFactory;
  class HReader;
  
  
  
  /**
   * @brief Delphes analysis library parts
   *
   */
  namespace HDelphes{
      
      class HClonesArray;
      class HClonesArraySnowmass;
      class HFile;
      class HEvent;
      class HJet;
      class HLepton;
      class HParticle;
      
  }
  
  /**
   * @brief Parton level analysis library parts
   *
   */
  namespace HParton{
      
      class HClonesArray;
      class HFile;
      class HEvent;
      class HParticle;
      
  }
  
  /**
   * @brief Pgs analysis library parts
   *
   */
  namespace HPgs{
      
      class HClonesArray;
      class HFile;
      class HEvent;
      class HLepton;
      class HJet;
      
  }

}

/**
 * @brief general base class for HAnalysis
 *
 */
class Analysis::HObject
{

public:

    /**
     * @brief Constructor
     *
     */
    HObject();

    /**
     * @brief Destructor
     *
     */
    virtual ~HObject();

    /**
     * @brief Print a debug message
     *
     * @param  Severity of the debug message
     * @param  Description of the debug message
     * @return void
     */
    void Print(const int Severity, const string Description) const;

    /**
     * @brief Print a debug messages
     *
     * @param  Severity of the debug message
     * @param  Description of the debug message
     * @param  Value relevant to the debug message
     *
     * @return void
     */
    template<typename TValue>
    void Print(const int Severity, const string Description, const TValue Value) const {

        if (Severity <= DebugLevel) {

            Printer(Description);
            std::cout << " " << Value << std::endl;

        }

    }

    /**
     * @brief Print a debug messages
     *
     * @param  Severity of the debug message
     * @param  Description of the debug message
     * @param  Value relevant to the debug message
     * @param  Value2 relevant to the debug message
     *
     * @return void
     */
    template<typename TValue, typename TValue2>
    void Print(const int Severity, const string Description, const TValue Value, const TValue2 Value2) const {

        if (Severity <= DebugLevel) {

            const char Separator = ' ';
            const int FunctionWidth = 10;

            Printer(Description);

            std::cout << std::left << std::setw(FunctionWidth) << std::setfill(Separator) << Value;
            std::cout << std::left << std::setw(FunctionWidth) << std::setfill(Separator) << Value2;
            std::cout << std::endl;

        }

    }

    /**
     * @brief Print a debug messages
     *
     * @param  Severity of the debug message
     * @param  Description of the debug message
     * @param  Value relevant to the debug message
     * @param  Value2 relevant to the debug message
     * @param  Value3 relevant to the debug message
     *
     * @return void
     */
    template<typename TValue, typename TValue2, typename TValue3>
    void Print(const int Severity, const string Description, const TValue Value, const TValue2 Value2, const TValue3 Value3) const {

        if (Severity <= DebugLevel) {

            const char Separator = ' ';
            const int FunctionWidth = 10;

            Printer(Description);

            std::cout << std::left << std::setw(FunctionWidth) << std::setfill(Separator) << Value;
            std::cout << std::left << std::setw(FunctionWidth) << std::setfill(Separator) << Value2;
            std::cout << std::left << std::setw(FunctionWidth) << std::setfill(Separator) << Value3;
            std::cout << std::endl;

        }

    }

protected:

    template <typename TValue> int sgn(const TValue Value) const {

        return (TValue(0) < Value) - (Value < TValue(0));

    }

    /**
     * @brief Calcualte distance in eta phi space
     *
     * @param  Eta1
     * @param  Phi1
     * @param  Eta2
     * @param  Phi2
     * @return float distance
     */
    float GetDistance(const float, const float, const float, const float) const;

    /**
     * @brief Calcualte distance from center in eta phi space
     *
     * @param  Eta
     * @param  Phi
     * @return float distance
     */
    float GetDistance(const float, const float) const;

    /**
     * @brief Take care of phi angles around pi
     *
     * @param Phi angle
     * @param RefPhi reference angle
     * @return float Phi angle
     */
    float GetDeltaPhi(const float, const float) const;

    virtual inline string ClassName() const {

        return ("HObject");

    };

    enum HStatus {
        Stable = 1,
        Unstable = 2,
        Undefined = 3
    };

    enum HParticleId {
        EmptyId = 0, ///> 0
        DownId = 1,///> \f$d\f$
        UpId = 2,///> \f$u\f$
        StrangeId = 3,///> \f$s\f$
        CharmId = 4,///> \f$c\f$
        BottomId = 5,///> \f$b\f$
        TopId = 6,///> \f$t\f$
        ElectronId = 11,
        ElectronNeutrinoId= 12,
        MuonId = 13,
        MuonNeutrinoId = 14,
        TauLeptonId = 15,
        TauNeutrinoId = 16,
        GluonId = 21,
        PhotonId = 22,
        ZId = 23,
        WId = 24,
        HiggsId = 25,
        HeavyHiggsId = 35,
        IsrId = 81,
        MarkerId = 82,
        PionId = 211,
        RhoMesonId = 213,
        KMeson0Id = 311,
        KMeson0SId = 313,
        KMesonId = 321,
        KMesonSId = 323,
        DMesonId = 411,
        DMesonSId = 413,
        DMesonS2Id = 415,
        DMeson0Id = 421,
        DMesonS0Id = 423,
        BMeson0Id = 511,
        BMeson0SId = 513,
        BMesonId = 521,
        BMesonSId = 523,
        BMesonS0Id = 531,
        BMesonSS0Id = 533,
        DownDown1Id = 1103,
        UpDown0Id = 2101,
        UpDown1Id = 2103,
        DeltaBaryonId = 1114,
        NeutronId = 2112,
        UpUp1Id = 2203,
        ProtonId = 2212,
        DeltaBaryon2Id = 2224,
        CpvHiggsId = 5000000
    };

    string GetStringFromEnum(const int ParticleId) const;


    /**
     * @brief Debug level
     *
     * 0: Errors
     * 1: Analysis Information
     * 2: Event Information
     * 3: Detailed Information
     * 4: Step by Step Information
     *
     */
    int DebugLevel;

    /**
     * @brief A large number
     *
     */
    const int LargeNumber;

    /**
     * @brief Top quark mass
     *
     */
    const float TopMass;

    /**
     * @brief Higgs boson mass
     *
     */
    const float HiggsMass;

    /**
     * @brief W bsoson mass
     *
     */
    const float WMass;

    /**
     * @brief Tau lepton mass
     *
     */
    const float TauMass;

    /**
     * @brief Muon mass
     *
     */
    const float MuonMass;

    /**
     * @brief Electron mass
     *
     */
    const float ElectronMass;

    /**
     * @brief Initial user index
     *
     */
    const int EmptyUserIndex;

    const float OneSigma;

    const float TwoSigma;

    const float ThreeSigma;

private:

    /**
     * @brief Print Debug messages
     *
     * @param  string Function Name
     * @return void
     */
    void Printer(const string) const;

};


struct SortByPt {

    inline bool operator()(const TLorentzVector &Lorentzvector1,
                           const TLorentzVector &Lorentzvector2) {

        return (Lorentzvector1.Pt() > Lorentzvector2.Pt());

    }

};

struct SortJetByPt {

    inline bool operator()(const PseudoJet &Jet1,
                           const PseudoJet &Jet2) {

        return (Jet1.pt() > Jet2.pt());

    }

};

template <typename TParticle>
class SortByMass
{

public:

    inline bool operator()(const TParticle &Particle1,
                           const TParticle &Particle2) {

        return (Particle1.m() > Particle2.m());

    }

};


class SortJetByMass
{

public:

    inline bool operator()(const PseudoJet &PseudoJet1,
                           const PseudoJet &PseudoJet2) {

        return (PseudoJet1.m() > PseudoJet2.m());

    }

};

// template <typename Template1, typename Template2>
struct SortPairs {

//     inline bool operator()(const pair<Template1, Template2> &Pair1,
//                            const pair<Template1, Template2> &Pair2) {
//
//         return (Pair1.second > Pair2.second);
//
//     }

    inline bool operator()(const std::pair<int, float> &Pair1,
                           const std::pair<int, float> &Pair2) {

        return (Pair1.second < Pair2.second);

    }

};

struct SortJetByEta {

    inline bool operator()(const PseudoJet &Jet1,
                           const PseudoJet &Jet2) {

        return (Jet1.eta() > Jet2.eta());

    }

};


struct SortJetByDistance {

    inline bool operator()(const PseudoJet &Jet1,
                           const PseudoJet &Jet2) {

        return (Jet1.eta() > Jet2.eta());

    }

};

class HRange
{

private:

    int Last;

    int Iter;

public:

    HRange(int End):
        Last(End),
        Iter(0)
    {}

    // Iterable functions
    const HRange &begin() const {
        return *this;
    }
    const HRange &end() const {
        return *this;
    }

    // Iterator functions
    bool operator!=(const HRange &) const {
        return Iter < Last;
    }
    void operator++() {
        ++Iter;
    }
    int operator*() const {
        return Iter;
    }

};

#endif

