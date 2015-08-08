#pragma once

#include <map>
#include <unordered_map>

#include "fastjet/PseudoJet.hh"
#include "fastjet/JetDefinition.hh"

#include "Identification.hh"
#include "Constituent.hh"
#include "FourVector.hh"

namespace analysis {

/**
 * @brief Jet infos subclassed from Fastjet
 *
 */
class JetInfoFamily: public Identification, public fastjet::PseudoJet::UserInfoBase {

public:

    /**
     * @brief Constructor
     *
     */
    JetInfoFamily();

    JetInfoFamily(float bdt);

    void AddDaughter(int daughter);

    std::unordered_map<Family, float> FamilyFractions();

    void ExtractFraction(int id);

    void ExtractAbsFraction(int id);

    void ExtractFraction(int id, int mother_id);

    void PrintAllInfos(const Severity severity) const;

    void PrintAllconstituentInfos(const Severity severity) const;

    void PrintAllFamInfos(const Severity severity) const;

    float MaximalFraction() const;

    float Fraction(int id) const;

    int MaximalId() const;

    void AddFamily(const Family& family, float weight);

    void ExtractFamilyFraction();

    Family MaximalFamily();

private:

    void AddParticle(int constituent_id, float weight);

    void AddParticle(Id constituent_id, float weight);

    float GetWeightSum() const;

    std::unordered_map<Family, float> family_fractions_;

    std::map<int, float> id_fractions_;

};

}