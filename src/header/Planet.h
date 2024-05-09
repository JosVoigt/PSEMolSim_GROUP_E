#pragma once

#include <array>

#include "Particle.h"

class Planet : public Particle {
   public:
    Planet(Particle &p);
    std::array<double, 3> calculateForce(Particle &other) override;
};