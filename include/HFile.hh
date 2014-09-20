# ifndef HFile_hh
# define HFile_hh

# include "HObject.hh"

/**
 * @brief Class dealing with pathes to delphes and the root files
 *
 * ProcessFolder has to be set
 *
 */
class HFile : public HObject
{

public:

    /**
     * @brief constructor defining default path
     *
     */
    explicit HFile();

    /**
     * @brief constructor defining default path
     *
     */
    explicit HFile(TString Process);

    /**
     * @brief constructor defining default path
     *
     */
    explicit HFile(TString Process, TString Run);

    /**
     * @brief destructor
     *
     */
    virtual ~HFile();

    /**
     * @brief Path path to the MadGraph installation
     *
     */
    static TString BasePath;

    /**
     * @brief Process name used in the file path
     *
     */
    TString ProcessFolder;

    /**
     * @brief Run name use in the file path
     *
     */
    TString RunFolder;

    /**
     * @brief Tag string used in the file name
     *
     */
    TString TagString;

    /**
     * @brief String containing the name of the root tree
     *
     */
    static TString TreeString;

    /**
     * @brief Name of Process
     *
     */
    TString Title;

    /**
     * @brief Crosssection of the event
     *
     */
    float Crosssection;

    /**
     * @brief Error of the Crosssection
     *
     */
    float Error;

    /**
     * @brief Compose file path
     *
     * @return TString file path
     */
    virtual TString GetFilePath() = 0;

    virtual TString GetTreeName() = 0;
    
    static TString FileSuffix;

protected:

    virtual TString MadGraphFilePath() {

        return (BasePath + ProcessFolder + "/Events/" + RunFolder + "/");

    };
    

private:

    void  SetVariables();

    virtual TString ClassName() {
        return ("HFile");
    };

};

class HFileParton : public HFile
{

public:
    
    using HFile::HFile;

    /**
     * @brief Compose file path
     *
     * @return TString file path
     */
    TString GetFilePath();

    TString GetTreeName();

private:
    
    virtual TString ClassName() {
        return ("HFileFlat");
    };

};

class HFilePgs : public HFile
{

public:
    
    using HFile::HFile;

    /**
     * @brief Compose file path
     *
     * @return TString file path
     */
    TString GetFilePath();

    TString GetTreeName();

private:
    virtual TString ClassName() {
        return ("HFileFlat");
    };

};

class HFileDelphes : public HFile
{

public:
    using HFile::HFile;

    /**
     * @brief Compose file path
     *
     * @return TString file path
     */
    TString GetFilePath();

    TString GetTreeName();

private:
    virtual TString ClassName() {
        return ("HFileDelphes");
    };

};

class HFileFolder : public HFile
{

public:
    using HFile::HFile;

    /**
     * @brief Compose file path
     *
     * @return TString file path
     */
    TString GetFilePath();

    TString GetTreeName();
        
private:
    virtual TString ClassName() {
        return ("HFileFolder");
    };

};

# endif
