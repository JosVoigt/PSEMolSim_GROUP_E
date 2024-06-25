#pragma once

#include <vector>

#include "particles/Particle.h"

/**
 * @brief Interface for particle spawner classes
 *
 * Definition of an interface for particle spawner classes. This is used to ensure inheriting classes implement the method
 * spawnParticles(ParticleContainer& particle_container) according to our definition.
 */
class ParticleSpawner {
   public:
    /**
     * @brief Spawns particles in the given container
     * @param particles A vector of particles to spawn
     * @return Number of particles spawned
     * Spawns particles in the given container.
     */
    virtual int spawnParticles(std::vector<Particle>& particles) const = 0;

    /**
     * @brief Estimate the number of particles to be spawned
     *
     * returns the number of particles to be spawned by this spawner
     * this can be used to reserve enought memory in the particle container
     */
    [[nodiscard]] virtual size_t getEstimatedNumberOfParticles() const = 0;
};