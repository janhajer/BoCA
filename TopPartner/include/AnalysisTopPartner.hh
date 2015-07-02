#pragma once

#include "Analysis.hh"

namespace analysis{

namespace toppartner {

/**
 *
 * @brief Top partner analysis
 *
 * \author Jan Hajer
 *
 */
class Analysis : public analysis::Analysis
{

public:

  void RunFast();

  void RunNormal();

  void RunFull();

  void RunFullTagger();

  Analysis(Tagger &tagger);

protected:

  void SetFiles(const Tag tag);

   std::string ProjectName() const;

  std::string ProcessName();

  /**
   * @brief Maximal number of Entries to analyse
   *
   */
   int EventNumberMax() const;

  virtual  std::string FilePath() const;


private:

  int PassPreCut(const Event &event);

  void RunTagger(analysis::Stage stage);

  void RunFactory();

  void RunReader();

  void RunReaderTagger();

  std::string PathName(const std::string &file_name) const;

};

}

}
