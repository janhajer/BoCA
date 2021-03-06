#include <cmath>
#include "boca/external/QJets.hh"
#include "boca/external/QJetsBaseExtras.hh"

namespace qjets
{

bool JetDistanceCompare::operator()(const jet_distance& lhs, const jet_distance& rhs) const
{
    return lhs.dij > rhs.dij;
}

QJets::QJets(double zcut, double dcut_fctr, double exp_min, double exp_max, double rigidity, double truncation_fctr)
    : _zcut(zcut),
      _dcut_fctr(dcut_fctr),
      _exp_min(exp_min),
      _exp_max(exp_max),
      _rigidity(rigidity),
      _dcut(-1.),
      _truncation_fctr(truncation_fctr),
      _rand_seed_set(false)
{
}

void QJets::SetRandSeed(unsigned seed)
{
    _rand_seed_set = true;
    _seed = seed;
}

bool QJets::JetUnmerged(int num) const
{
    return _merged_jets.find(num) == _merged_jets.end();
}

bool QJets::JetsUnmerged(const jet_distance& jd) const
{
    return JetUnmerged(jd.j1) && JetUnmerged(jd.j2);
}

jet_distance QJets::GetNextDistance()
{
    std::vector< std::pair<jet_distance, double> > popped_distances;
    double norm(0.);
    jet_distance ret;
    ret.j1 = -1;
    ret.j2 = -1;
    ret.dij = -1.;
    auto dmin_set = false;
    auto dmin = 0.;

    while (!_distances.empty()) {
        auto dist = _distances.top();
        _distances.pop();
        if (JetsUnmerged(dist)) {
            if (!dmin_set) {
                dmin = dist.dij;
                dmin_set = true;
            }
            auto weight = std::exp(-_rigidity * (dist.dij - dmin) / dmin);
            popped_distances.push_back(std::make_pair(dist, weight));
            norm += weight;
            if (weight / norm < _truncation_fctr)
                break;
        }
    }

    auto rand = Rand();
    auto tot_weight = 0.;
    for (auto it = popped_distances.begin(); it != popped_distances.end(); it++) {
        tot_weight += (*it).second / norm;
        if (tot_weight >= rand) {
            ret = (*it).first;
            Omega *= ((*it).second);
            break;
        }
    }

    // repopulate in reverse (maybe quicker?)
    for (auto it = popped_distances.rbegin(); it != popped_distances.rend(); it++)
        if (JetsUnmerged((*it).first))
            _distances.push((*it).first);

    return ret;
}

void QJets::Cluster(fastjet::ClusterSequence& cs)
{
    Omega = 1.;
    auto extras = new QJetsBaseExtras();
    ComputeDCut(cs);

    // Populate all the distances
    ComputeAllDistances(cs.jets());
    auto jd = GetNextDistance();

    while (!_distances.empty() && jd.dij != -1.) {
        if (!Prune(jd, cs)) {
            //      _merged_jets.push_back(jd.j1);
            //      _merged_jets.push_back(jd.j2);
            _merged_jets[jd.j1] = true;
            _merged_jets[jd.j2] = true;

            int new_jet;
            cs.plugin_record_ij_recombination(jd.j1, jd.j2, 1., new_jet);
            assert(JetUnmerged(new_jet));
            ComputeNewDistanceMeasures(cs, new_jet);
        } else {
            auto j1pt = cs.jets()[jd.j1].perp();
            auto j2pt = cs.jets()[jd.j2].perp();
            if (j1pt > j2pt) {
                //      _merged_jets.push_back(jd.j2);
                _merged_jets[jd.j2] = true;
                cs.plugin_record_iB_recombination(jd.j2, 1.);
            } else {
                //      _merged_jets.push_back(jd.j1);
                _merged_jets[jd.j1] = true;
                cs.plugin_record_iB_recombination(jd.j1, 1.);
            }
        }
        jd = GetNextDistance();
    }

    extras->SetWeight(Omega);
    cs.plugin_associate_extras(extras);

    // merge remaining jets with beam
    auto num_merged_final = 0;
    for (unsigned i = 0 ; i < cs.jets().size(); i++)
        if (JetUnmerged(i)) {
            num_merged_final++;
            cs.plugin_record_iB_recombination(i, 1.);
        }
    assert(num_merged_final < 2);
}

void QJets::ComputeDCut(fastjet::ClusterSequence& cs)
{
    // assume all jets in cs form a single jet.  compute mass and pt
    fastjet::PseudoJet sum(0., 0., 0., 0.);
    for (auto it = cs.jets().begin(); it != cs.jets().end(); it++)
        sum += (*it);
    _dcut = 2. * _dcut_fctr * sum.m() / sum.perp();
}

bool QJets::Prune(jet_distance& jd, fastjet::ClusterSequence& cs)
{
    auto pt1 = cs.jets()[jd.j1].perp();
    auto pt2 = cs.jets()[jd.j2].perp();
    auto sum_jet = cs.jets()[jd.j1] + cs.jets()[jd.j2];
    auto sum_pt = sum_jet.perp();
    auto z = std::min(pt1, pt2) / sum_pt;
    auto d = std::sqrt(cs.jets()[jd.j1].plain_distance(cs.jets()[jd.j2]));
    return (d > _dcut) && (z < _zcut);
}

void QJets::ComputeAllDistances(std::vector<fastjet::PseudoJet> const& inp)
{
    for (unsigned i = 0 ; i < inp.size() - 1; i++) {
        // jet-jet distances
        for (auto j = i + 1 ; j < inp.size(); j++) {
            jet_distance jd;
            jd.j1 = i;
            jd.j2 = j;
            if (jd.j1 != jd.j2) {
                jd.dij = d_ij(inp[i], inp[j]);
                _distances.push(jd);
            }
        }
    }
}

void QJets::ComputeNewDistanceMeasures(fastjet::ClusterSequence& cs, unsigned new_jet)
{
    // jet-jet distances
    for (unsigned i = 0; i < cs.jets().size(); i++)
        if (JetUnmerged(i) && i != new_jet) {
            jet_distance jd;
            jd.j1 = new_jet;
            jd.j2 = i;
            jd.dij = d_ij(cs.jets()[jd.j1], cs.jets()[jd.j2]);
            _distances.push(jd);
        }
}

double QJets::d_ij(fastjet::PseudoJet const& v1, const  fastjet::PseudoJet& v2) const
{
    auto p1 = v1.perp();
    auto p2 = v2.perp();
    auto ret = std::pow(std::min(p1, p2), _exp_min) * std::pow(std::max(p1, p2), _exp_max) * v1.squared_distance(v2);
    assert(!std::isnan(ret));
    return ret;
}

double QJets::Rand()
{
    auto ret = 0.;
    if (_rand_seed_set) ret = static_cast<double>(rand_r(&_seed)) / RAND_MAX;
    else ret = static_cast<double>(rand()) / RAND_MAX;
    return ret;
}

}
