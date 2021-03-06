/**
 * Copyright (C) 2015-2016 Jan Hajer
 */
#pragma once

#include <boost/range/algorithm/sort.hpp>
#include <boost/units/cmath.hpp>
#include "boca/physics/Id.hh"

namespace boca
{

template<typename Multiplet_1_, typename Multiplet_2_>
std::vector<Multiplet_1_> SortedByMinDeltaRTo(std::vector<Multiplet_1_> multiplets, Multiplet_2_ const& jet)
{
    return boost::range::sort(multiplets, [& jet](Multiplet_1_ const & multiplet1, Multiplet_1_ const & multiplet2) {
        return multiplet1.DeltaRTo(jet) < multiplet2.DeltaRTo(jet);
    });
}

template <class Multiplet>
std::vector<Multiplet> SortedByMaxDeltaRap(std::vector<Multiplet> multiplets)
{
    return boost::range::sort(multiplets, [](const Multiplet & multiplet_1, const Multiplet & multiplet_2) {
        return (multiplet_1.DeltaRap() > multiplet_2.DeltaRap());
    });
}

template <class Multiplet>
std::vector<Multiplet> SortedByMassTo(std::vector<Multiplet> multiplets, Mass const& mass)
{
    return boost::range::sort(multiplets, [mass](const Multiplet & multiplet_1, const Multiplet & multiplet_2) {
        return boost::units::abs(multiplet_1.Mass() - mass) < boost::units::abs(multiplet_2.Mass() - mass);
    });
}

template <class Multiplet>
std::vector<Multiplet> SortedByMassTo(std::vector<Multiplet> multiplets, Id id)
{
    auto mass = MassOf(id);
    return SortedByMassTo(multiplets, mass);
}

template <class Multiplet>
std::vector<Multiplet> SortedByMass(std::vector<Multiplet> multiplets)
{
    return boost::range::sort(multiplets, [](Multiplet const & multiplet_1, Multiplet const & multiplet_2) {
        return multiplet_1.Mass() > multiplet_2.Mass();
    });
}

template <class Multiplet>
std::vector<Multiplet> SortedByPt(std::vector<Multiplet> multiplets)
{
    return boost::range::sort(multiplets, [](Multiplet const & multiplet_1, Multiplet const & multiplet_2) {
        return multiplet_1.Pt() > multiplet_2.Pt();
    });
}

template <class Multiplet>
std::vector<Multiplet> SortedByRap(std::vector<Multiplet> multiplets)
{
    return boost::range::sort(multiplets, [](Multiplet const & multiplet_1, Multiplet const & multiplet_2) {
        return boost::units::abs(multiplet_1.Rap()) > boost::units::abs(multiplet_2.Rap());
    });
}

template <class Multiplet>
std::vector<Multiplet> SortedByPhi(std::vector<Multiplet> multiplets)
{
  return boost::range::sort(multiplets, [](Multiplet const & multiplet_1, Multiplet const & multiplet_2) {
    return multiplet_1.Phi() > multiplet_2.Phi();
  });
}

/**
 * @brief sort vector of jets with largest bdt at the front
 *
 */
template <typename Multiplet>
std::vector<Multiplet> SortedByBdt(std::vector<Multiplet>  multiplets)
{
    return boost::range::sort(multiplets, [](Multiplet const & multiplet_1, Multiplet const & multiplet_2) {
        return multiplet_1.Bdt() > multiplet_2.Bdt();
    });
}

}
