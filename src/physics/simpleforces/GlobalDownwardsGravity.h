#pragma once

#include "physics/simpleforces/SimpleForceSource.h"

/**
 *  @brief Class to calculate the global downwards gravity force.
 */
class GlobalDownwardsGravity : public SimpleForceSource {
   private:
    double g;

   public:
    /**
     * @brief Constructor for the GlobalDownwardsGravity class
     * @param g Gravitational acceleration
     */
    explicit GlobalDownwardsGravity(double g);

    /**
     * @brief Calculates the downwards gravity force on a particle using the given gravitational acceleration
     * @param p Particle
     * @return std::array<double, 3UL> Force vector
     */
    std::array<double, 3UL> calculateForce(Particle& p) const override;

    /**
     * @brief Returns "Gravity" as name of the force
     */
    explicit operator std::string() const override;

    /**
     * @brief Sets the gravitational acceleration
     *
     * @param g Gravitational acceleration
     */
    void setGravitationalAcceleration(double g);
};
