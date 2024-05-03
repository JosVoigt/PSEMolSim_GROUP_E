#pragma once

#include <array>

#include "Force.h"

class Planet : public Force {
   public:
    Planet() = default;
    std::array<double, 3> calculateForce(Particle &p1,
                                         Particle &p2) const override;
};