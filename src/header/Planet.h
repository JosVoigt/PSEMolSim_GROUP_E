#pragma once

#include <array>

#include "Particle.h"

class Planet : Particle {
   public:
    std::array<double, 3> calculateForce(Particle &other) override;
};