# include "HJetDelphes.hh"

HJetDelphes::HJetDelphes()
{

    Print(1, "Constructor");

//     DebugLevel = 4;

}

HJetDelphes::~HJetDelphes()
{

    Print(1, "Destructor");

}

bool HJetDelphes::GetJets()
{

    return GetJets(0, 0);

}

bool HJetDelphes::GetTaggedJets(const HJetTag *const NewJetTag)
{
    JetTag = NewJetTag;

    return GetJets(1, 0);

}

bool HJetDelphes::GetStructuredJets()
{

    return GetJets(0, 1);

}

bool HJetDelphes::GetJets(const bool Tagging, const bool Structure)
{

    Print(2, "Get Jets", ClonesArrays->JetSum());

    for (int JetNumber = 0; JetNumber < ClonesArrays->JetSum(); ++JetNumber) {

        Print(4, "Jet Number", JetNumber);
        const Jet *const JetClone = (Jet *)ClonesArrays->JetClonesArray->At(JetNumber);

        if (Structure) {

            JetVector.push_back(GetConstituents(JetClone));

        } else {

            JetVector.push_back(GetPseudoJet(const_cast<Jet *>(JetClone)->P4()));

        }

        if (Tagging) {

            JetVector.back().set_user_info(new HJetInfo(GetJetId(JetClone)));
            JetVector.back().set_user_index(JetVector.back().user_info<HJetInfo>().GetMaximalId());


        }

        GetDelphesTags(JetClone);

//         JetVector[JetNumber].user_info<HJetInfo>().PrintAllInfos();
        Print(0, "Tag", JetVector[JetNumber].user_info<HJetInfo>().GetMaximalId(), JetVector[JetNumber].user_info<HJetInfo>().GetMaximalFraction());

    }
    Print(0, "Loop finished");
    return 1;

}

void HJetDelphes::GetDelphesTags(const Jet *const JetClone)
{
    Print(2, "Get taggs");

    if (JetClone->TauTag == 1) {

        Print(4, "Has Tau Tag");

        GetTau(JetClone);

    } else if (JetClone->BTag > 0) {

        Print(4, "Has B Tag");

        BottomLorentzVectorVector.push_back(const_cast<Jet *>(JetClone)->P4());
        BottomJetVector.push_back(GetPseudoJet(const_cast<Jet *>(JetClone)->P4()));

    } else {

        JetLorentzVectorVector.push_back(const_cast<Jet *>(JetClone)->P4());

    }

}


void HJetDelphes::GetTau(const Jet *const JetClone)
{

    Print(2, "TauTagCalculations");

    if (JetClone->Charge == - 1) {

        TauLorentzVectorVector.push_back(const_cast<Jet *>(JetClone)->P4());
        Print(3, "Tau Jet");

    } else if (JetClone->Charge == 1) {

        AntiTauLorentzVectorVector.push_back(const_cast<Jet *>(JetClone)->P4());
        Print(3, "Anti Tau Jet");

    } else Print(0, "Jet Charge: ", JetClone->Charge);

}

int HJetDelphes::GetMotherId(const TObject *const Object)
{


    Print(2, "Get Mother Id", ClonesArrays->ParticleSum());

    if (Object->IsA() != GenParticle::Class()) {

        Print(0, "Object is", Object->ClassName());
        return 0;

    }

    GenParticle *ParticleClone = (GenParticle *) Object;

    int BranchId = EmptyId;
    int BranchCharge = 0;

    HBranchStruct BranchStruct;

    BranchStruct.Id = EmptyId;
    BranchStruct.Charge = 0;

    BranchStruct = GetMotherId(ParticleClone, BranchStruct);

    const int MotherId = BranchStruct.Id;

//     if (MotherId == EmptyId)
    Print(3, "Mother Id", MotherId);
//     DebugLevel = 0;

    return MotherId;

}




HBranchStruct HJetDelphes::GetMotherId(GenParticle *ParticleClone, HBranchStruct BranchStruct)
{

    Print(2, "Get Mother Id", ParticleClone->PID);
    const int EmptyPosition = -1;

    while (ParticleClone->M1 != EmptyPosition) {

//         if (ParticleClone->PID == -6) DebugLevel = 4;
        Print(3, "Particle Id", ParticleClone->PID);

//         BranchId = GetBranchId(ParticleClone->PID, BranchId);
        BranchStruct = JetTag->GetBranchId(int(ParticleClone->PID), BranchStruct);

//         if (ParticleClone->PID == HeavyHiggsId) {
//
//             Print(0, "PID", ParticleClone->PID);
//
//             if (ParticleClone->D1 != -1) {
//
//                 GenParticle *Daughter1 = (GenParticle *) ClonesArrays->ParticleClonesArray->At(ParticleClone->D1);
//                 Print(0, "D1", Daughter1->PID);
//
//             }
//             if (ParticleClone->D2 != -1) {
//                 GenParticle *Daughter2 = (GenParticle *) ClonesArrays->ParticleClonesArray->At(ParticleClone->D2);
//
//                 Print(0, "D2", Daughter2->PID);
//             }
//
//
//         }

        if (ParticleClone->M2 != EmptyPosition) {

            Print(3, "Mother 2");
            ParticleClone = (GenParticle *) ClonesArrays->ParticleClonesArray->At(ParticleClone->M2);
            BranchStruct = GetMotherId(ParticleClone, BranchStruct);

        }

        Print(4, "Mother 1");


        if (ParticleClone->M1 > ClonesArrays->ParticleSum()) {

            Print(0, "Faulty eMotherPositionvent");
            continue;

        }

        ParticleClone = (GenParticle *) ClonesArrays->ParticleClonesArray->At(ParticleClone->M1);

    }

    Print(3, "Branch Id", BranchStruct.Id);

    if (sgn(BranchStruct.Charge) != sgn(BranchStruct.Id)) Print(0, "Branch Sign Problem", BranchStruct.Charge);

    return BranchStruct;

}

PseudoJet HJetDelphes::GetConstituents(const Jet *const JetClone) const
{

    Print(2, "Get Constituents");

    vector<PseudoJet> ConstituentsVector;

    for (int ConstituentNumber = 0; ConstituentNumber < JetClone->Constituents.GetEntriesFast(); ++ConstituentNumber) {

        const TObject *const Object = JetClone->Constituents.At(ConstituentNumber);

        if (Object == 0) continue;

        ConstituentsVector.push_back(GetConstituentJet(Object));

    }

    return fastjet::join(ConstituentsVector);

}

bool HJetDelphes::GetEFlow(const HJetDetails JetDetails)
{
    Print(2, "Get EFlow");

    if (ClonesArrays->EFlowTrackClonesArray) GetTrackEFlow(JetDetails);
    if (ClonesArrays->EFlowPhotonClonesArray) GetPhotonEFlow(JetDetails);
    if (ClonesArrays->EFlowNeutralHadronClonesArray) GetHadronEFlow(JetDetails);
    if (ClonesArrays->EFlowMuonClonesArray) GetMuonEFlow(JetDetails);

    Print(3, "Number of EFlow Jet", EFlowJetVector.size());

    return 1;

}


void HJetDelphes::GetTrackEFlow(const HJetDetails JetDetails)
{
    Print(2, "Get Track EFlow", ClonesArrays->EFlowTrackSum());

    if (ClonesArrays->ElectronSum() > 0) Print(4, "Number of Electons", ClonesArrays->ElectronSum());
    if (ClonesArrays->MuonSum() > 0) Print(4, "Number of Muons", ClonesArrays->MuonSum());

    for (int EFlowTrackNumber = 0; EFlowTrackNumber < ClonesArrays->EFlowTrackSum() ; ++EFlowTrackNumber) {

        const Track *const EFlowTrackClone = (Track *) ClonesArrays->EFlowTrackClonesArray->At(EFlowTrackNumber);

        if (Isolation || TaggingIsolation) {

            bool Isolated = GetIsolation<Electron>(EFlowTrackClone, ClonesArrays->ElectronClonesArray);
            if (Isolated) Isolated = GetIsolation<Muon>(EFlowTrackClone, ClonesArrays->MuonClonesArray);
            if (!Isolated) continue;

        }

        EFlowJetVector.push_back(GetPseudoJet(const_cast<Track *>(EFlowTrackClone)->P4()));

        if (Tagging || TaggingIsolation) {

            EFlowJetVector.back().set_user_index(GetMotherId(EFlowTrackClone->Particle.GetObject()));
            Print(4, "Track EFlow Id", EFlowJetVector.back().user_index());

        }

    }


}

void HJetDelphes::GetPhotonEFlow(const HJetDetails JetDetails)
{
    Print(2, "Get Photon EFlow", ClonesArrays->EFlowPhotonSum());

    if (ClonesArrays->PhotonSum() > 0) Print(3, "Number of Photons", ClonesArrays->PhotonSum());
    for (int EFlowPhotonNumber = 0; EFlowPhotonNumber < ClonesArrays->EFlowPhotonSum() ; ++EFlowPhotonNumber) {

        const Tower *const EFlowPhotonClone = (Tower *) ClonesArrays->EFlowPhotonClonesArray->At(EFlowPhotonNumber);

        if (Isolation || TaggingIsolation) {

            bool Isolated = GetIsolation<Photon>(EFlowPhotonClone, ClonesArrays->PhotonClonesArray);
            if (!Isolated) continue;

        }

        EFlowJetVector.push_back(GetPseudoJet(const_cast<Tower *>(EFlowPhotonClone)->P4()));

        if (Tagging || TaggingIsolation) {

            EFlowJetVector.back().set_user_info(new HJetInfo(GetJetId(EFlowPhotonClone)));
            EFlowJetVector.back().set_user_index(EFlowJetVector.back().user_info<HJetInfo>().GetMaximalId());
            Print(4, "Photon EFlow Id", EFlowJetVector.back().user_index());

        }

    }

}

void HJetDelphes::GetHadronEFlow(const HJetDetails JetDetails)
{

    Print(2, "Get Hadron EFlow", ClonesArrays->EFlowNeutralHadronSum());

    for (int HadronNumber = 0; HadronNumber < ClonesArrays->EFlowNeutralHadronSum(); ++HadronNumber) {

        const Tower *const HadronClone = (Tower *) ClonesArrays->EFlowNeutralHadronClonesArray->At(HadronNumber);

        EFlowJetVector.push_back(GetPseudoJet(const_cast<Tower *>(HadronClone)->P4()));
        if (Tagging || TaggingIsolation) {

            EFlowJetVector.back().set_user_info(new HJetInfo(GetJetId(HadronClone)));
            EFlowJetVector.back().set_user_index(EFlowJetVector.back().user_info<HJetInfo>().GetMaximalId());
            Print(4, "Hadron EFlow Id", EFlowJetVector.back().user_index());

        }

    }

}

void HJetDelphes::GetMuonEFlow(const HJetDetails JetDetails)
{

    Print(2, "Get Muon EFlow", ClonesArrays->EFlowMuonSum());

    for (int MuonNumber = 0; MuonNumber < ClonesArrays->EFlowMuonSum(); ++MuonNumber) {

        const Muon *const EFlowMuonClone = (Muon *) ClonesArrays->EFlowMuonClonesArray->At(MuonNumber);

        if (Isolation || TaggingIsolation) {

            bool Isolated = GetIsolation<Muon>(EFlowMuonClone, ClonesArrays->MuonClonesArray);
            if (!Isolated) continue;

        }

        EFlowJetVector.push_back(GetPseudoJet(const_cast<Muon *>(EFlowMuonClone)->P4()));

        if (Tagging || TaggingIsolation) {
            EFlowJetVector.back().set_user_index(GetMotherId(EFlowMuonClone->Particle.GetObject()));
            Print(4, "Muon EFlow Id", EFlowJetVector.back().user_index());
        }

    }

}



void HJetDelphes::GetGenJet()
{

    Print(2, "GetGenJet", ClonesArrays->GenJetSum());

    for (int GenJetNumber = 0; GenJetNumber <  ClonesArrays->GenJetSum(); ++GenJetNumber) {

        const Jet *const GenJetClone = (Jet *) ClonesArrays->GenJetClonesArray->At(GenJetNumber);

        GenJetVector.push_back(GetPseudoJet(const_cast<Jet *>(GenJetClone)->P4()));

    }

}
