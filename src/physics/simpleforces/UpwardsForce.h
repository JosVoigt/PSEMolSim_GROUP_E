#pragma once

#include <array>
#include <vector>

#include "particles/Particle.h"

class UpwardsForce {
private:
    std::array<double, 3> force;
    std::vector<int> target_particles;
    double end_time;

public:
    explicit UpwardsForce(std::array<double, 3> force, std::vector<int> target_particles, double end_time);

    /**
     * @brief Calculates the small upwards force for the given particles
     * @param particles Particle vector
     */
    void calculateForce(std::vector<Particle>& particles) const;

    explicit operator std::string() const;

    [[nodiscard]] double getEndTime() const { return end_time; }
};
