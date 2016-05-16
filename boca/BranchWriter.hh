/**
 * Copyright (C) 2015-2016 Jan Hajer
 */
#pragma once

#include <mutex>

#include "boca/io/FileWriter.hh"
#include "boca/multivariant/Reader.hh"
#include "boca/Branches.hh"
#include "boca/File.hh"
#include "boca/generic/Debug.hh"

namespace boca
{

template<typename Tagger_>
class BranchWriter
{

public:

    BranchWriter(boca::Phase& phase, boca::File& file, boca::FileWriter& file_writer, Tagger_& tagger) :
        object_sum_(0),
        event_sum_(0),
        phase_(phase),
        import_file_(file),
        tagger_(tagger),
        reader_(phase.Stage()) {
        tree_writer_ = &file_writer.NewTree(Import().Title());
        switch (Phase().Stage()) {
        case Stage::trainer : tagger_.NewBranch(TreeWriter(), Phase().Stage());
            break;
        case Stage::reader : reader_.NewBranch(TreeWriter(), Phase().Stage());
            break;
        }
        tree_branch_ = &(tree_writer_->NewBranch<InfoBranch>(tagger_.WeightBranchName()));
    }

    ~BranchWriter() {
      std::cout << "PreCut ratio: " << RoundToDigits(static_cast<double>(object_sum_.load()) / event_sum_.load()) << std::endl;
      if (object_sum_.load()) TreeWriter().Write();
    }

    void SafeEntry() {
        InfoBranch info_branch(Import());
        info_branch.SetEventNumber(event_sum_.load());
        std::lock_guard<std::mutex> lock(write_mutex);
        TreeBranch().AddEntry(info_branch);
        TreeWriter().Fill();
    }
    void Increment(int number) {
        object_sum_ += number;
//         if (number > 0)
        ++event_sum_;
        Debug("event: ", event_sum_.load(), "object: ", object_sum_.load());
    }

    boca::Reader<Tagger_> Reader() const {
        return reader_;
    }

    Tagger_ Tagger() const {
        return tagger_;
    }

    bool KeepGoing(std::function<long(Stage)> const& event_number_max) const {
      return object_sum_.load() < event_number_max(Phase().Stage());
    }

    boca::Phase Phase() const {
        return phase_;
    }

    boca::File Import() const {
        return import_file_;
    }

private:

    boca::TreeBranch& TreeBranch() {
        return *tree_branch_;
    }

    boca::TreeWriter& TreeWriter() {
        return *tree_writer_;
    }

    std::atomic<long> object_sum_;

    std::atomic<long> event_sum_;

    boca::Phase phase_;

    File& import_file_;

    Tagger_& tagger_;

    boca::Reader<Tagger_> reader_;

    boca::TreeWriter* tree_writer_;

    boca::TreeBranch* tree_branch_;

    std::mutex write_mutex;

};

}


