#pragma once

#include "physics/pairwiseforces/PairwiseForceSource.h"

/**
 *  @brief Class to calculate gravitational forces between particles. Implements the interface PairwiseForceSource.
 *
 * Implementation of the force calculation to simulate gravity forces between particles.
 * This class implements the interface PairwiseForceSource.
 */
class GravitationalForce : public PairwiseForceSource {
   public:
    /**
     * @brief Calculates the gravitational forces between two particles
     *
     * @param p Particle
     * @param q Particle
     * @return Gravitational force exerted by q on p
     *
     * Calculates the gravitational force which q exerts on p
     */
    std::array<double, 3UL> calculateForce(Particle& p, Particle& q) const override;

    /**
     * @brief Returns "Gravity" as name of the force
     */
    explicit operator std::string() const override;
};
