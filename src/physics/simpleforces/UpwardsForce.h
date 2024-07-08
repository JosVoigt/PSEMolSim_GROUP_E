#pragma once

#include "SimpleForceSource.h"

class UpwardsForce {
private:
    std::array<double, 3> force;
    std::vector<int> target_particles;
    double end_time;

public:
    UpwardsForce(std::array<double, 3> force, std::vector<int>& target_particles, double end_time);
    /**
     * @brief Calculates the small upwards force for the given particle
     * @param p Particle vector
     * @param time Current time
     */
    void calculateForce(std::vector<Particle>& p, double time) const;
};
