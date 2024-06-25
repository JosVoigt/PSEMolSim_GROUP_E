#pragma once

#include <array>

#include "particles/spawners/ParticleSpawner.h"

/**
 * @brief Class to spawn particles in a cuboid. Implements the interface ParticleSpawner.
 *
 * Implementation of the particle spawning in a cuboid.
 * It is possible to specify the lower left corner of the cuboid, the grid dimensions, the spacing between neighboring particles, the mass
 * of the particles, the initial velocity of the particles and the type of the particles.
 */
class CuboidSpawner : public ParticleSpawner {
   private:
    /**
     * @brief Defines the lower left corner where the cuboid will be spawned
     */
    const std::array<double, 3> lower_left_corner;

    /**
     * @brief Defines how big the cuboid will be. Each entry defines the number of particles in the respective direction.
     */
    const std::array<int, 3> grid_dimensions;

    /**
     * @brief Defines the spacing between neighboring particles in the cuboid
     */
    const double grid_spacing;

    /**
     * @brief Defines the mass of the particles in the cuboid
     */
    const double mass;

    /**
     * @brief Defines the type of the particles in the cuboid
     */
    const int type;

    /**
     * @brief Defines the Lennard-Jones epsilon parameter of the particles in the cuboid
     */
    double epsilon;

    /**
     * @brief Defines the Lennard-Jones sigma parameter of the particles in the cuboid
     */
    double sigma;

    /**
     * @brief Defines the initial velocity of the particles in the cuboid
     */
    const std::array<double, 3> initial_velocity;

    /**
     * @brief Defines whether to use the third dimension
     */
    const bool third_dimension;

    /**
     * @brief Defines the initial temperature of the particles in the cuboid
     */
    const double initial_temperature;

   public:
    /**
     * @brief Constructor
     * @param lower_left_corner Lower left corner of the cuboid
     * @param grid_dimensions Grid dimensions
     * @param grid_spacing Spacing between neighboring particles
     * @param mass Mass of the particles
     * @param initial_velocity Initial velocity of the particles, the velocity is then jittered by a Maxwell-Boltzmann distribution
     * @param type Type of the particles in the cuboid
     * @param epsilon Lennard-Jones epsilon parameter of the particles in the cuboid
     * @param sigma Lennard-Jones sigma parameter of the particles in the cuboid
     * @param third_dimension Whether to spawn particles in the third dimension
     * @param initial_temperature Initial temperature of the particles
     *
     * Constructor to initialize the cuboid spawner. The velocity of the particles is jittered by a Maxwell-Boltzmann distribution.
     */
    CuboidSpawner(const std::array<double, 3>& lower_left_corner, const std::array<int, 3>& grid_dimensions, double grid_spacing,
                  double mass, const std::array<double, 3>& initial_velocity, int type, double epsilon = 1.0, double sigma = 1.2,
                  bool third_dimension = true, double initial_temperature = 0.1);

    /**
     * @brief Spawns particles in the given container
     * @param particles A vector of particles to spawn
     * @return Number of particles spawned
     * Spawns particles in the given container.
     */
    int spawnParticles(std::vector<Particle>& particles) const override;

    /**
     * @brief Estimate the number of particles to be spawned
     *
     * returns the number of particles to be spawned by this spawner
     * this can be used to reserve enought memory in the particle container
     */
    [[nodiscard]] size_t getEstimatedNumberOfParticles() const override;
};