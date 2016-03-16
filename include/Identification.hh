/**
 * Copyright (C) 2015-2016 Jan Hajer
 */
#pragma once

#include <string>

namespace boca {

enum class Tag {
    background = 0,
    signal = 1
};

std::string Name(Tag tag);

class Identification {

public:

    Identification();

    virtual void SetBdt(double bdt);

    void SetBdt(double bdt_1, double bdt_2);

    virtual double Bdt() const;

    void SetTag(boca::Tag tag);

    void SetTag(boca::Tag tag_1, boca::Tag tag_2);

    boca::Tag Tag() const;

    static double InitialValue();

    /// largest bdt in front
    bool operator<(Identification const& multiplet) const;

private:

    double bdt_;

    boca::Tag tag_;

};

}

