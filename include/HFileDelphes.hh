# ifndef HFileDelphes_hh
# define HFileDelphes_hh

# include "TFile.h"
# include "TTree.h"
# include "ExRootAnalysis/ExRootTreeReader.h"

# include "HObject.hh"
# include "HClonesArray.hh"
# include "HFile.hh"

/**
 * @brief Delphes files
 *
 */
class hdelphes::HFile : public hanalysis::HFile
{

public:

    /**
     * @brief constructor defining default path
     *
     */
    HFile();

    /**
     * @brief constructor defining default path
     *
     */
    HFile(const std::string &Process);

    /**
     * @brief constructor defining default path
     *
     */
    HFile(const std::string &Process, const std::string &Run);

    /**
     * @brief Compose file path
     *
     * @return std::string file path
     */
    std::string GetFilePath() const;

//     std::string GetTreeName() const;

    hanalysis::HEvent *GetEvent();

    hanalysis::HClonesArray *GetClonesArrays();

protected:

    inline std::string NameSpaceName() const {
        return "HDelphes";
    }

    virtual inline std::string ClassName() const {
        return "HFile";
    };

private:

};


# endif