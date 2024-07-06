#pragma once

#include "particles/spawners/ParticleSpawner.h"

/**
 * @brief Class to spawn particles in a membrane. Implements the interface ParticleSpawner.
 *
 * Implementation of the particle spawning in a membrane.
 * It is possible to specify the lower left corner of the membrane, the grid dimensions, the spacing between neighboring particles, the mass
 * of the particles, the initial velocity of the particles and the type of the particles.
 */
class MembraneSpawner : public ParticleSpawner{
private:
    /**
     * @brief Defines the lower left corner where the membrane will be spawned
     */
    const std::array<double, 3> lower_left_corner;

    /**
     * @brief Defines how big the membrane will be. Each entry defines the number of particles in the respective direction.
     */
    const std::array<int, 3> grid_dimensions;

    /**
     * @brief Defines the spacing between neighboring particles in the membrane
     */
    const double grid_spacing;

    /**
     * @brief Defines the mass of the particles in the membrane
     */
    const double mass;

    /**
     * @brief Defines the type of the particles in the membrane
     */
    const int type;

    /**
     * @brief Defines the Lennard-Jones epsilon parameter of the particles in the membrane
     */
    double epsilon;

    /**
     * @brief Defines the Lennard-Jones sigma parameter of the particles in the membrane
     */
    double sigma;

    /**
     * @brief Defines the initial velocity of the particles in the membrane
     */
    const std::array<double, 3> initial_velocity;

    /**
     * @brief Defines whether to use the third dimension
     */
    const bool third_dimension;

    /**
     * @brief Defines the initial temperature of the particles in the membrane
     */
    const double initial_temperature;

    /**
     * @brief Defines straight neighbours of the particles in the membrane
     */
    std::vector<Particle*> straight_neighbours;

    /**
     * @brief Defines diagonal neighbours of the particles in the membrane
     */
    std::vector<Particle*> diagonal_neighbours;

public:
    /**
     * @brief Constructor
     * @param lower_left_corner Lower left corner of the membrane
     * @param grid_dimensions Grid dimensions
     * @param grid_spacing Spacing between neighboring particles
     * @param mass Mass of the particles
     * @param initial_velocity Initial velocity of the particles
     * @param type Type of the particles
     * @param epsilon Lennard-Jones epsilon parameter
     * @param sigma Lennard-Jones sigma parameter
     * @param third_dimension Whether to use the third dimension
     * @param initial_temperature Initial temperature of the particles
     * @param straight_neighbours Straight neighbours of the particles
     * @param diagonal_neighbours Diagonal neighbours of the particles
     */
    MembraneSpawner(const std::array<double, 3>& lower_left_corner, const std::array<int, 3>& grid_dimensions, double grid_spacing,
                  double mass, const std::array<double, 3>& initial_velocity, int type, double epsilon = 1.0, double sigma = 1.2,
                  bool third_dimension = true, double initial_temperature = 0.1,
                    std::vector<Particle*> straight_neighbours = {}, std::vector<Particle*> diagonal_neighbours = {});

    /**
     * @brief Spawns particles in the membrane
     * @param particles Vector of particles
     * @return Number of particles spawned
     */
    int spawnParticles(std::vector<Particle>& particles) const override;

    /**
     * @brief Gets the estimated number of particles
     * @return Estimated number of particles
     */
    size_t getEstimatedNumberOfParticles() const override;
};