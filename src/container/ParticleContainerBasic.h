#pragma once

#include <list>
#include <vector>

#include "Particle.h"
#include "container/ParticleContainerInterface.h"

/**
 * \brief
 *  A container for particles
 */
class ParticleContainer : public ParticleContainerInterface{
   private:
    /**
     * The array containing all particles
     */
    std::vector<Particle> particleArray;

    /**
     * The amount of particles in the container
     */
    std::size_t length;

   public:
    /**
     * \brief
     *  Creates a new ParticleContainer with a given amount of particles
     * \param count
     *  The amount of particles in the container
     * \param init
     * A list of particles to initialize the container with
     */
    ParticleContainer(std::size_t count, std::list<Particle>& init);

    ParticleContainer() = default;

    /**
     * \brief
     *  Destructor for the ParticleContainer
     */
    ~ParticleContainer() = default;

    typedef std::vector<Particle>::iterator Iterator;

    /**
     * \brief
     *  Returns an iterator to the first particle in the container
     * \return
     *  An iterator to the first particle in the container
     */
    Iterator begin() { return particleArray.begin(); }

    /**
     * \brief
     *  Returns an iterator to the end of the container
     * \return
     *  An iterator to the end of the container
     */
    Iterator end() { return particleArray.end(); }

    /**
     * \brief
     *  Returns the amount of particles in the container.
     * \return
     *  The amount of particles in the container
     */
    [[nodiscard]] std::size_t size() const;

	std::vector<Particle> preprocessParticles();
	std::vector<Particle> insertIfRelevantParticles(Particle& p);
	void updateParticles();
};
