#include "JetInfo.hh"

#include "Predicate.hh"
#include "delphes/Delphes.hh"
#include "Debug.hh"

namespace analysis {

JetInfo::JetInfo()
{
    Debug();
}

JetInfo::JetInfo(float bdt)
{
    SetBdt(bdt);
}

void JetInfo::SetDelphesTags(const ::delphes::Jet& jet)
{
    SetBTag(jet.BTag);
    SetTauTag(jet.TauTag);
}

JetInfo::JetInfo(const ::delphes::Jet& jet)
{
    SetDelphesTags(jet);
}

JetInfo::JetInfo(bool b_tag)
{
    SetBTag(b_tag);
}

// JetInfo::JetInfo(bool b_tag, int charge)
// {
//   SetBTag(b_tag);
//   SetCharge(charge);
// }

JetInfo::JetInfo(bool b_tag, bool tau_tag)
{
    SetBTag(b_tag);
    SetTauTag(tau_tag);
}

JetInfo::JetInfo(int charge)
{
    SetCharge(charge);
}

JetInfo::JetInfo(const Constituent& constituent)
{
    constituents_.emplace_back(constituent);
    if (VertexResultion(constituent))
        displaced_constituents_.emplace_back(constituent);
}

JetInfo::JetInfo(const Constituent& constituent, int charge)
{
    constituents_.emplace_back(constituent);
    if (VertexResultion(constituent))
        displaced_constituents_.emplace_back(constituent);
    SetCharge(charge);
}

JetInfo::JetInfo(const std::vector<Constituent>& constituents)
{
    constituents_ = constituents;
    displaced_constituents_ = ApplyVertexResolution(constituents);
}

JetInfo::JetInfo(const std::vector<Constituent>& constituents, const std::vector<Constituent>& displaced_constituents)
{
    constituents_ = constituents;
    displaced_constituents_ = displaced_constituents;
}

void JetInfo::AddConstituent(const Constituent& constituent)
{
    if (VertexResultion(constituent))
        constituents_.emplace_back(constituent);
}

void JetInfo::AddConstituents(const std::vector<Constituent>& constituents)
{
    constituents_ = Join(constituents_, constituents);
    displaced_constituents_ = Join(displaced_constituents_, ApplyVertexResolution(constituents));
}

void JetInfo::SetConstituents(const std::vector<Constituent>& constituents)
{
  constituents_ = constituents;
  displaced_constituents_ = ApplyVertexResolution(constituents);
}

void JetInfo::AddDaughter(int daughter)
{
    if (!constituents().empty()) {
        Error(constituents().size(), constituents().front().family().particle().id());
        constituents().front().family().AddDaughter(daughter);
        return;
    }
    Error("No constituent");
}

std::vector<Constituent> JetInfo::constituents() const
{
    return constituents_;
}

std::vector<Constituent> JetInfo::displaced_constituents() const
{
    return displaced_constituents_;
}

std::unordered_map<Family, float> JetInfo::FamilyFractions()
{
    return family_fractions_;
}

int JetInfo::VertexNumber() const
{
    return displaced_constituents_.size();
//     return ApplyVertexResolution().size();
}

void JetInfo::AddFamily(const Family& family, float weight)
{
    Debug(family.particle().id(), family.mother_1().id(), weight);
    family_fractions_[family] += weight;
}

void JetInfo::ExtractFamilyFraction()
{
    Info();
    for (const auto& constituent : constituents())
        family_fractions_[constituent.family()] += constituent.Momentum().Pt();
}

struct SortPairs {
    template <typename Template>
    bool operator()(const std::pair<Template, float>& pair_1, const std::pair<Template, float>& pair_2)
    {
        return (pair_1.second < pair_2.second);
    }
};

Family JetInfo::MaximalFamily()
{
    Debug();
    return std::max_element(family_fractions_.begin(), family_fractions_.end(), SortPairs())->first;
}

void JetInfo::AddParticle(int constituent_id, float weight)
{
    Debug(constituent_id, weight);
    id_fractions_[constituent_id] += weight;
    Detail(id_fractions_[constituent_id]);
}

void JetInfo::AddParticle(Id constituent_id, float weight)
{
    Debug(Name(constituent_id), weight);
    id_fractions_[int(constituent_id)] += weight;
    Detail(id_fractions_[int(constituent_id)]);
}

void JetInfo::ExtractFraction(int id)
{
    Info(id);
    ExtractFamilyFraction();
    for (const auto& pair : family_fractions_) {
        if (pair.first.particle().id() == id || pair.first.mother_1().id() == id)
            AddParticle(id, pair.second);
        else if (pair.first.particle().id() == -id || pair.first.mother_1().id() == -id)
            AddParticle(-id, pair.second);
        else
            AddParticle(pair.first.particle().id(), pair.second);
    }
}

void JetInfo::ExtractFraction(int id, int mother_id)
{
    Info(id, mother_id);
    for (const auto& pair : family_fractions_) {
        if (std::abs(pair.first.particle().id()) == id && std::abs(pair.first.mother_1().id()) == mother_id)
            AddParticle(pair.first.particle().id(), pair.second);
        else
            AddParticle(Id::isr, pair.second);
    }
}

void JetInfo::ExtractAbsFraction(int id)
{
    Info(id);
    ExtractFamilyFraction();
    for (const auto& pair : family_fractions_) {
        if (std::abs(pair.first.particle().id()) == id || std::abs(pair.first.mother_1().id()) == id)
            AddParticle(id, pair.second);
        else
            AddParticle(pair.first.particle().id(), pair.second);
    }
}

float JetInfo::GetWeightSum() const
{
    Debug(id_fractions_.size());
    float weight_sum = std::accumulate(begin(id_fractions_), end(id_fractions_), 0.0, [](float previous, const std::pair<int, float>& pair) {
        return (previous + pair.second);
    });
    Detail(weight_sum);
    return weight_sum;
}

float JetInfo::Fraction(int id) const
{
    Info(id);
    if (!id_fractions_.count(id))
        return 0;
    if (GetWeightSum() == 0)
        return 0;
    return (id_fractions_.at(id) / GetWeightSum());
}

float JetInfo::MaximalFraction() const
{
    Info();
    std::pair<int, float> maximal_weight = *std::max_element(id_fractions_.begin(), id_fractions_.end(), SortPairs());
    if (GetWeightSum() == 0)
        return 0;
    else
        return (maximal_weight.second / GetWeightSum());
}

int JetInfo::MaximalId() const
{
    Debug();
    return std::max_element(id_fractions_.begin(), id_fractions_.end(), SortPairs())->first;
}

void JetInfo::PrintAllInfos(const Severity) const
{
    Debug();
//     for (auto pair = id_fractions_.begin(); pair != id_fractions_.end(); ++pair) {
//         if (GetWeightSum() == 0) Print(severity, "Jet Fraction", Name((*pair).first), 0);
//         else Print(severity, "Jet Fraction", Name((*pair).first), (*pair).second / GetWeightSum());
//     }
}

void JetInfo::PrintAllconstituentInfos(const Severity) const
{
    Debug();
//     for (const auto &constituent : constituents())
//         Print(severity, "Jet Fraction", Name(constituent.family().particle().id()), Name(constituent.family().mother_1().id()), constituent.family().particle().Momentum.Pt(), constituent.family().mother_1().Momentum.Pt());
}
void JetInfo::PrintAllFamInfos(const Severity) const
{
    Debug();
//     for (const auto & family_fraction : family_fractions_)
//         Print(severity, "Family Fraction", Name(family_fraction.first.particle().id()), Name(family_fraction.first.mother_1().id()), family_fraction.first.particle().Momentum.Pt(), family_fraction.first.mother_1().Momentum.Pt());
}

fastjet::PseudoJet JetInfo::VertexJet() const
{
    Jets jets;
    for (const auto& consituent : displaced_constituents_)
        jets.emplace_back(PseudoJet(consituent.Momentum()));
    return fastjet::join(jets);
}

float JetInfo::SumDisplacement() const
{
    Debug();
    if (displaced_constituents_.empty()) return 0;
    return std::accumulate(displaced_constituents_.rbegin(), displaced_constituents_.rend(), 0, [](float result, const Constituent& constituent){
      return (result + constituent.Position().Vect().Perp());
    });
}

float JetInfo::MeanDisplacement() const
{
    Debug();
    if (displaced_constituents_.empty()) return 0;
    return SumDisplacement() / displaced_constituents_.size();
}

float JetInfo::MaxDisplacement() const
{
    Debug();
    if (displaced_constituents_.empty())
        return 0;
    return (*std::max_element(displaced_constituents_.begin(), displaced_constituents_.end(), [](const Constituent& constituent_1, const Constituent& constituent_2){
      return constituent_1.Position().Vect().Perp() > constituent_2.Position().Vect().Perp();
    } )).Position().Vect().Perp();
}

float JetInfo::VertexMass() const
{
    Debug();
    float vertex_mass = std::accumulate(displaced_constituents_.begin(), displaced_constituents_.end(), LorentzVector(),[](const LorentzVector& momentum,const Constituent &constituent){
      return momentum + constituent.Momentum();
    }).M();
    Debug(vertex_mass);
    if (vertex_mass < DetectorGeometry::VertexMassMin())
        return 0;
    return vertex_mass;
}

float JetInfo::VertexEnergy() const
{
    Debug();
    float vertex_energy = std::accumulate(displaced_constituents_.begin(), displaced_constituents_.end(), Constituent()).Momentum().E();
    return vertex_energy;
}

std::vector<Constituent> JetInfo::ApplyVertexResolution(std::vector<Constituent> constituents) const
{
    Debug(constituents.size());
    if (constituents.empty())
        return constituents;
    std::vector <Constituent> displaced_constituents;
    for (const auto& constituent : constituents) if (VertexResultion(constituent))
            displaced_constituents.emplace_back(constituent);
    Debug(displaced_constituents.size());
    return displaced_constituents;
}

bool JetInfo::VertexResultion(const Constituent& constituent) const
{
    Debug();
    return (constituent.Position().Vect().Perp() > DetectorGeometry::TrackerDistanceMin() & constituent.Position().Vect().Perp() < DetectorGeometry::TrackerDistanceMax() & std::abs(constituent.Momentum().Rapidity()) < DetectorGeometry::TrackerEtaMax());
}

float JetInfo::ElectroMagneticRadius(const fastjet::PseudoJet& jet) const
{
    float energy = 0;
    float weight = 0;
    for (const auto& constituent : constituents()) if (constituent.sub_detector() == SubDetector::photon) {
            energy += constituent.Momentum().Et();
            weight += constituent.Momentum().Et() * jet.delta_R(fastjet::PseudoJet(constituent.Momentum()));
        }
    if (energy == 0)
        return 0;
    else
        return weight / energy;
}

float JetInfo::TrackRadius(const fastjet::PseudoJet& jet) const
{
    float energy = 0;
    float weight = 0;
    for (const auto& constituent : constituents()) if (constituent.sub_detector() == SubDetector::track) {
            energy += constituent.Momentum().Et();
            weight += constituent.Momentum().Et() * jet.delta_R(fastjet::PseudoJet(constituent.Momentum()));
        }
    if (energy == 0)
        return 0;
    else
        return weight / energy;
}

struct WrongDetector {
    WrongDetector(const SubDetector sub_detector)
    {
        sub_detector_ = sub_detector;
    }
    bool operator()(const Constituent& constituent)
    {
        return constituent.sub_detector() != sub_detector_;
    }
    SubDetector sub_detector_;
};

float JetInfo::LeadingTrackMomentumFraction() const
{
    std::vector<Constituent> consts = constituents();
    consts.erase(std::remove_if(consts.begin(), consts.end(), WrongDetector(SubDetector::track)), consts.end());
    std::sort(consts.begin(), consts.end(), [](const Constituent& constituent_1, const Constituent& constituent_2){
      return (constituent_1.Momentum().Pt() > constituent_2.Momentum().Pt());
    });
    float sum = std::accumulate(consts.rbegin(), consts.rend(), 0, [](float result, const Constituent& constituent){
      return (result + constituent.Momentum().Pt());
    });
    return consts.front().Momentum().Pt() / sum;
}

float JetInfo::CoreEnergyFraction(const fastjet::PseudoJet& jet) const
{
    float energy = 0;
    float core_energy = 0;
    for (const auto& constituent : constituents()) if (constituent.sub_detector() == SubDetector::photon) {
            energy += constituent.Momentum().Et();
            if (jet.delta_R(fastjet::PseudoJet(constituent.Momentum())) < 0.2)
                core_energy += constituent.Momentum().Et();
        }
    if (energy == 0)
        return 0;
    else
        return core_energy / energy;
}

float JetInfo::ElectroMagneticFraction() const
{
    float em_energy = 0;
    float energy = 0;
    for (const auto& constituent : constituents()) {
        energy += constituent.Momentum().Et();
        if (constituent.sub_detector() == SubDetector::photon)
            em_energy += constituent.Momentum().Et();
    }
    if (energy == 0)
        return 0;
    else
        return em_energy / energy;
}

float JetInfo::ClusterMass() const
{
    fastjet::PseudoJet jet;
    for (const auto& constituent : constituents()) if (constituent.sub_detector() == SubDetector::photon)
            jet += constituent.Momentum();
    return jet.m();
}

float JetInfo::TrackMass() const
{
    fastjet::PseudoJet jet;
    for (const auto& constituent : constituents()) if (constituent.sub_detector() == SubDetector::track)
            jet += constituent.Momentum();
    return jet.m();
}

int JetInfo::Charge() const
{
    Debug();
    if (charge_ != LargeNumber())
        return charge_;
    int charge = std::accumulate(constituents_.begin(), constituents_.end(), 0, [](int charge, const Constituent & constituent) {
        return charge + constituent.charge();
    });
    return charge;
}

bool SortByBdt::operator()(const fastjet::PseudoJet& jet_1, const fastjet::PseudoJet& jet_2)
{
    return jet_1.user_info<analysis::JetInfo>().Bdt() > jet_2.user_info<analysis::JetInfo>().Bdt();
}

void JetInfo::SecondayVertex() const
{
    auto leading = std::max_element(constituents_.begin(), constituents_.end(), [](Constituent s1, Constituent s2) {
        return s1.Momentum().Pt() < s2.Momentum().Pt();
    });
    float x = (*leading).Position().X();
    float y = (*leading).Position().Y();
    float radius = (*leading).Position().Perp() / 2;
    std::vector<Constituent> vertices;
    auto it = std::copy_if(constituents_.begin(), constituents_.end(), vertices.begin(), [&](const Constituent & s1) {
        return (s1.Position().X() < x + radius && s1.Position().X() > x - radius && s1.Position().Y() < y + radius && s1.Position().Y() > y - radius);
    });
    vertices.resize(std::distance(vertices.begin(), it));
}

InfoRecombiner::InfoRecombiner(fastjet::RecombinationScheme recombination_scheme): fastjet::JetDefinition::DefaultRecombiner(recombination_scheme) {}

std::string InfoRecombiner::description() const
{
    return fastjet::JetDefinition::DefaultRecombiner::description() + " (with user info)";
}

void InfoRecombiner::recombine(const fastjet::PseudoJet& jet_1, const fastjet::PseudoJet& jet_2, fastjet::PseudoJet& jet) const
{
    fastjet::JetDefinition::DefaultRecombiner::recombine(jet_1, jet_2, jet);
    jet.set_user_info(new JetInfo(Join(jet_1.user_info<JetInfo>().constituents(), jet_2.user_info<JetInfo>().constituents()), Join(jet_1.user_info<JetInfo>().displaced_constituents(), jet_2.user_info<JetInfo>().displaced_constituents())));
}

}
