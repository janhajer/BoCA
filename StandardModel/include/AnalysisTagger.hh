#pragma once

#include "Analysis.hh"

namespace analysis
{

namespace standardmodel
{

enum class Process
{
    tt, tt_lep, tt_had, hh, hh_bb, zz, zz_bb, ww, bb, cc, qq, gg
};

enum class Collider
{
    LHC, FHC, LE
};

/**
 *
 * @brief Tagger ananlysis
 *
 * @author Jan Hajer
 *
 */
class Analysis : public analysis::Analysis
{

protected:

    Analysis(Tagger &tagger);

    void SetFiles(const Tag tag);

    std::string ProjectName() const;

    virtual std::string FilePath() const;

    virtual void NewSignalFile(const Process process);

    virtual void NewBackgroundFile(const Process process);

    virtual std::string FileName(const Process process) const;

    virtual int LowerPtCut() const;

    virtual int UpperPtCut() const;

    virtual int MadGraphCut() const;

    virtual int LowerQuarkCut() const;

    virtual int UpperQuarkCut() const;

    virtual int EventNumberMax() const;

    virtual Collider collider_type() const;

    virtual int BackgroundFileNumber() const;

    virtual File BackgroundFile(const Process process) const;

    virtual File BackgroundFile(const Process process, const int file_sum) const;

    virtual std::string BackgroundTree(const Process Process) const;

    virtual std::string ColliderName(const Collider collider) const;

    virtual std::string ProcessName(const Process process) const;

    virtual std::string NiceName(const Process process) const;

};

}

}
