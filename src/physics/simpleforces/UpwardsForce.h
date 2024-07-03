#pragma once

#include "SimpleForceSource.h"

class UpwardsForce : public SimpleForceSource {

    /**
     * @brief Calculates the small upwards force for the given particle
     * @param p Particle
     * @return std::array<double, 3UL> Force vector
     */
    std::array<double, 3UL> calculateForce(Particle& p) const override;
};
