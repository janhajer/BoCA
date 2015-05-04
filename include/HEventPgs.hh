# ifndef HEventPgs_hh
# define HEventPgs_hh

# include "HEvent.hh"
# include "HLeptonPgs.hh"
# include "HJetPgs.hh"

/**
 * @brief event topology for PGS events
 *
 */
class hanalysis::hpgs::HEvent : public hanalysis::HEvent
{

public:

    /**
    * @brief constructor
    *
    */
    HEvent();

    /**
     * @brief destructor
     *
     */
    ~HEvent();

    void NewEvent(const hanalysis::ClonesArrays &ClonesArrays);

private:

    inline std::string NameSpaceName() const {
      return "hpgs";
    }

    inline std::string ClassName() const {
        return "HEvent";
    };

};

#endif

