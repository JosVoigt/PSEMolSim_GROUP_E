#pragma once

#include "particles/spawners/ParticleSpawner.h"

/**
 * @brief Class to spawn particles in a Sphere. Implements the interface ParticleSpawner.
 *
 * Implementation of the particle spawning in a cuboid.
 * It is possible to specify the center of the sphere, the radius of the sphere, the spacing between neighboring particles, the mass
 * of the particles, the initial velocity of the particles and the type of the particles.
 *
 * The radius is given in terms of number of particles along the radius.
 */
class SphereSpawner : public ParticleSpawner {
   private:
    /**
     * @brief Defines the center of the sphere
     */
    const std::array<double, 3> center;

    /**
     * @brief Defines the radius of the sphere
     */
    const int sphere_radius;

    /**
     * @brief Defines the spacing between neighboring particles in the sphere
     */
    const double grid_spacing;

    /**
     * @brief Defines the mass of the particles in the sphere
     */
    const double mass;

    /**
     * @brief Defines the type of the particles in the sphere
     */
    const int type;

    /**
     * @brief Defines the Lennard-Jones epsilon parameter of the particles in the sphere
     */
    const double espilon;

    /**
     * @brief Defines the Lennard-Jones sigma parameter of the particles in the sphere
     */
    const double sigma;

    /**
     * @brief Defines the initial velocity of the particles in the sphere
     */
    const std::array<double, 3> initial_velocity;

    /**
     * @brief Defines the initial temperature of the particles in the sphere
     */
    const double initial_temperature;

    /**
     * @brief Defines whether to use the third dimension
     */
    const bool third_dimension;

   public:
    /**
     * @brief Constructor
     * @param center Center of the sphere
     * @param sphere_radius Radius of the sphere [number of particles along the radius]
     * @param grid_spacing Spacing between neighboring particles
     * @param mass Mass of the particles
     * @param initial_velocity Initial velocity of the particles, the velocity is then jittered by a Maxwell-Boltzmann distribution
     * @param type Type of the particles in the cuboid
     * @param epsilon Lennard-Jones epsilon parameter of the particles in the cuboid
     * @param sigma Lennard-Jones sigma parameter of the particles in the cuboid
     * @param third_dimension Number of dimensions of the sphere
     * @param initial_temperature Inital temperature of the particles
     *
     * Constructor to initialize the cuboid spawner. The velocity of the particles is jittered by a Maxwell-Boltzmann distribution.
     */
    SphereSpawner(const std::array<double, 3>& center, int sphere_radius, double grid_spacing, double mass,
                  const std::array<double, 3>& initial_velocity, int type, double epsilon = 1.0, double sigma = 1.2,
                  bool third_dimension = true, double initial_temperature = 0.1);

    /**
     * @brief Spawns particles in the given container
     * @param particles A vector of particles to spawn
     * @return Number of particles spawned
     * Spawns particles in the given container.
     */
    int spawnParticles(std::vector<Particle>& particles) const override;

    /**
     * @brief Returns the number of particles to be spawned
     * @return Number of particles to be spawned
     *
     * Returns the number of particles to be spawned.
     */
    [[nodiscard]] size_t getEstimatedNumberOfParticles() const override;
};