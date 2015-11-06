
#include "delphes/Delphes.hh"
#include "exroot/ExRootAnalysis.hh"

#include "TreeReader.hh"
// #define INFORMATION
#include "Debug.hh"

namespace boca
{

std::string BranchName(Branch branch)
{
    Info0;
    switch (branch) {
    case Branch::particle : return "Particle";
    case Branch::photon : return "Photon";
    case Branch::electron : return "Electron";
    case Branch::muon : return "Muon";
    case Branch::jet : return "Jet";
    case Branch::missing_et : return "MissingET";
    case Branch::track : return "Track";
    case Branch::tower : return "Tower";
    case Branch::e_flow_track : return "EFlowTrack";
    case Branch::e_flow_photon : return "EFlowPhoton";
    case Branch::e_flow_neutral_hadron : return "EFlowNeutralHadron";
    case Branch::e_flow_muon : return "EFlowMuon";
    case Branch::gen_jet : return "GenJet";
    case Branch::scalar_ht : return "ScalarHT";
    case Branch::tau : return "Tau";
        Default("Branch", "");
    }
}

std::mutex TreeReader::mutex_;

TreeReader::TreeReader(TChain& chain)
{
    Info0;
    chain_ = &chain;
    source_ = Source::delphes;
    NewElements();
    std::lock_guard<std::mutex> guard(mutex_);
    tree_reader_.SetTree(chain_);
}

long TreeReader::GetEntries() const
{
    Info0;
    std::lock_guard<std::mutex> guard(mutex_);
    return tree_reader_.GetEntries(false);
}

bool TreeReader::ReadEntry(long number)
{
    Info(number);
    std::lock_guard<std::mutex> guard(mutex_);
    bool valid = tree_reader_.SetEntry(number) == TTreeReader::kEntryValid;
    for (auto & pair : map) pair.second->Fill();
    return valid;
}

void TreeReader::NewElements()
{
    switch (source_) {
    case Source::delphes :
        NewElement<delphes::GenParticle>(Branch::particle);
        NewElement<delphes::Electron>(Branch::electron);
        NewElement<delphes::Muon>(Branch::muon);
        NewElement<delphes::Photon>(Branch::photon);
        NewElement<delphes::Jet>(Branch::jet);
        NewElement<delphes::MissingET>(Branch::missing_et);
        NewElement<delphes::Track>(Branch::track);
        NewElement<delphes::Tower>(Branch::tower);
        NewElement<delphes::Track>(Branch::e_flow_track);
        NewElement<delphes::Tower>(Branch::e_flow_photon);
        NewElement<delphes::Tower>(Branch::e_flow_neutral_hadron);
        NewElement<delphes::Jet>(Branch::gen_jet);
        NewElement<delphes::ScalarHT>(Branch::scalar_ht);
        break;
    case Source::pgs :
        NewElement<exroot::Photon>(Branch::photon);
        NewElement<exroot::Electron>(Branch::electron);
        NewElement<exroot::Muon>(Branch::muon);
        NewElement<exroot::Jet>(Branch::jet);
        NewElement<exroot::MissingET>(Branch::missing_et);
        NewElement<exroot::GenJet>(Branch::gen_jet);
        NewElement<exroot::Tau>(Branch::tau);
        break;
    case Source::parton :
        NewElement<exroot::GenParticle>(Branch::particle);
        break;
    default : break;
    }
}

bool TreeReader::Has(Branch branch) const
{
    return map.find(branch) != map.end();
}

}