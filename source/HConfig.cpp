# include "HConfig.hh"


    HConfig::HConfig() {
        config_ = new libconfig::Config;
        ReadConfig("Standard");
    }

    HConfig::HConfig(const std::string &ConfigName) {
        config_ = new libconfig::Config;
        ReadConfig(ConfigName);
    }

    HConfig::~HConfig() {
        delete config_;
    }

    // in GeV
    inline int HConfig::Mass() const {
        return mass_;
    }

           // in GeV
           inline int HConfig::PreCut() const {
        return pre_cut_;
    }

    inline int HConfig::eventNumberMax() const {
        return event_number_max_;
    };

    inline int HConfig::BackgroundFileNumber() const {
        return background_file_number_;
    }

    inline HColliderType HConfig::ColliderType() const {
        return collider_type_;
    }

    // in GeV
    inline int HConfig::Mass_()  {
        try {
            mass_ = config_->lookup("Mass");
        } catch (const libconfig::SettingNotFoundException &SettingNotFoundException) {
            std::cerr << "No 'Mass' setting in configuration file." << std::endl;
            throw;
        } catch (const libconfig::SettingTypeException &SettingTypeException) {
            std::cerr << "'Mass' setting has wrong type." << std::endl;
            throw;
        }
    }

    // in GeV
    inline int HConfig::PreCut_()  {
        try {
            pre_cut_ = config_->lookup("PreCut");
        } catch (const libconfig::SettingNotFoundException &SettingNotFoundException) {
            std::cerr << "No 'PreCut' setting in configuration file." << std::endl;
            throw;
        } catch (const libconfig::SettingTypeException &SettingTypeException) {
            std::cerr << "'PreCut' setting has wrong type." << std::endl;
            throw;
        }
    }

    inline int HConfig::eventNumberMax_()  {
        try {
            event_number_max_ = config_->lookup("eventNumberMax");
        } catch (const libconfig::SettingNotFoundException &SettingNotFoundException) {
            std::cerr << "No 'eventNumberMax' setting in configuration file." << std::endl;
            throw;
        } catch (const libconfig::SettingTypeException &SettingTypeException) {
            std::cerr << "'eventNumberMax' setting has wrong type." << std::endl;
            throw;
        }
    };

    inline int HConfig::BackgroundFileNumber_()  {
        try {
            background_file_number_ = config_->lookup("BackgroundFileNumber");
        } catch (const libconfig::SettingNotFoundException &SettingNotFoundException) {
            std::cerr << "No 'BackgroundFileNumber' setting in configuration file." << std::endl;
            throw;
        } catch (const libconfig::SettingTypeException &SettingTypeException) {
            std::cerr << "'BackgroundFileNumber' setting has wrong type." << std::endl;
            throw;
        }
    }

    inline HColliderType HConfig::ColliderType_()  {
        try {
            std::string Collider = config_->lookup("ColliderType");
            if (Collider == "LHC") collider_type_ = LHC;
            else if (Collider == "LE") collider_type_ = LE;
            else if (Collider == "FHC") collider_type_ = FHC;
        } catch (const libconfig::SettingNotFoundException &SettingNotFoundException) {
            std::cerr << "No 'ColliderType' setting in configuration file." << std::endl;
            throw;
        } catch (const libconfig::SettingTypeException &SettingTypeException) {
            std::cerr << "'ColliderType' setting has wrong type." << std::endl;
            throw;
        }
        return LHC;
    }


    void HConfig::WriteConfig(const std::string &ConfigName) {
        std::string config_name = ConfigName + ".cfg";
//         std::string config_name = "Neutral.cfg";

        libconfig::Setting &root = config_->getRoot();
        libconfig::Setting &mass = root.add("Mass",  libconfig::Setting::TypeInt) = 1000;
        libconfig::Setting &pre_cut = root.add("PreCut",  libconfig::Setting::TypeInt) = 1000;
        libconfig::Setting &event_number_max = root.add("eventNumberMax",  libconfig::Setting::TypeInt) = 10000;
        libconfig::Setting &background_file_number = root.add("BackgroundFileNumber",  libconfig::Setting::TypeInt) = 1;
        libconfig::Setting &collider_type = root.add("ColliderType",  libconfig::Setting::TypeString) = "LE";
        try {
            config_->writeFile(config_name.c_str());
            std::cerr << "New configuration successfully written to: " << config_name << std::endl;
        } catch (const libconfig::FileIOException &FileIOException) {
            std::cerr << "I/O error while writing file: " << config_name << std::endl;
        }
    }

    void HConfig::ReadConfig(const std::string &ConfigName) {
        try {
            std::string config_name = ConfigName + ".cfg";
//             std::string config_name = "Neutral.cfg";
            config_->readFile(config_name.c_str());
        } catch (const libconfig::FileIOException &FileIOException) {
            std::cerr << "I/O error while reading file." << std::endl;
            WriteConfig(ConfigName);
        } catch (const libconfig::ParseException &ParseException) {
            std::cerr << "Parse error at " << ParseException.getFile() << ":" << ParseException.getLine() << " - " << ParseException.getError() << std::endl;
        }
        Mass_();
        PreCut_();
        eventNumberMax_();
        BackgroundFileNumber_();
        ColliderType_();
    }

