#pragma once

#include <memory>
#include <vector>

#include "particles/Particle.h"
#include "physics/pairwiseforces/PairwiseForceSource.h"
#include "physics/simpleforces/SimpleForceSource.h"

/**
 * @brief Interface for particle containers
 *
 * This class defines the interface for particle containers.
 * Further information about the implementation can be found in \ref ParticleContainers "Particle Containers"
 */
class ParticleContainer {
   public:
    /**
     * @brief Virtual destructor for correct deconstruction of inheriting classes
     */
    virtual ~ParticleContainer() = default;

    /**
     * @brief Adds a particle to the container
     *
     * @param p Particle to be added
     *
     * Adds a particle to the container.
     */
    virtual void addParticle(const Particle& p) = 0;

    /**
     * @brief Adds a particle to the container
     *
     * @param p Particle to be added
     *
     * Adds a particle to the container.
     */
    virtual void addParticle(Particle&& p) = 0;

    /**
     * @brief Prepares everything for the force calculations (must be called before applySimpleForces and applyPairwiseForces)
     */
    virtual void prepareForceCalculation() = 0;

    /**
     * @brief Applies the given simple force sources to the particles
     *
     * @param simple_force_sources List of simple force sources to be applied
     *
     * Applies the given simple force sources to the particles in the container.
     */
    virtual void applySimpleForces(const std::vector<std::shared_ptr<SimpleForceSource>>& simple_force_sources) = 0;

    /**
     * @brief Applies the given pairwise force sources to the particles
     *
     * @param pairwise_force_sources List of pairwise force sources to be applied
     *
     * Applies the given pairwise force sources to the particles in the container.
     * Uses newton's third law to calculate the forces between the particles in an optimized way.
     */
    virtual void applyPairwiseForces(const std::vector<std::shared_ptr<PairwiseForceSource>>& pairwise_force_sources) = 0;

    /**
     * @brief Reserves space for n particles
     *
     * @param n Amount of particles to store in the container
     */
    virtual void reserve(size_t n) = 0;

    /**
     * @brief Returns the number of particles in the container
     *
     * @return Number of particles in the container
     */
    [[nodiscard]] virtual size_t size() const = 0;

    /**
     * @brief Overload of the [] operator to access the particles in the container
     *
     * @param i
     * @return Particle lvalue reference to the particle at index i in the internal data structure
     */
    virtual Particle& operator[](int i) = 0;

    /**
     * @brief The begin iterator for the internal data structure.
     *
     * @return Iterator to the first particle
     */
    [[nodiscard]] virtual std::vector<Particle>::iterator begin() = 0;

    /**
     * @brief The end iterator for the internal data structure.
     *
     * @return Iterator to the end of the container
     */
    [[nodiscard]] virtual std::vector<Particle>::iterator end() = 0;

    /**
     * @brief The begin const iterator for the internal data structure.
     *
     * @return Const iterator to the first particle
     */
    [[nodiscard]] virtual std::vector<Particle>::const_iterator begin() const = 0;

    /**
     * @brief The end const iterator for the internal data structure.
     *
     * @return Const iterator to the end of the container
     */
    [[nodiscard]] virtual std::vector<Particle>::const_iterator end() const = 0;

    /**
     * @brief Returns a vector of all particles in the container
     * @return Vector of all particles in the container
     */
    [[nodiscard]] virtual const std::vector<Particle>& getParticles() const = 0;
};