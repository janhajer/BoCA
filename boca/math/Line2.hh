/**
 * Copyright (C) 2015-2016 Jan Hajer
 */
#pragma once

#include "boca/math/Line.hh"
#include "boca/math/Vector2.hh"

namespace boca
{

/**
 * @ingroup Math
 * @brief Line between two points in two dimensional space
 */
template<typename Value_>
using Line2 = Line<Vector2, Value_>;

}
