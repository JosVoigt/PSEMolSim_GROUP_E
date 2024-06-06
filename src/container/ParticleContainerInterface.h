#pragma once

#include <vector>
#include "Particle.h"

class ParticleContainerInterface {
 public:
    /**
     * \brief
     * Destructor
     */
    virtual ~ParticleContainerInterface() = default;

    /**
     * \brief
     * Returns the list of Particles in case they are needed for preprocessing purposes
     */
    virtual std::vector<Particle> preprocessParticles() = 0;

    /**
     * \brief
     * Returns a list of all particles in the container
     */
    virtual std::vector<Particle> retrieveRelevantParticles(Particle& particle) = 0;

    /**
     * \brief
     * Updates the location and state of the particles in the container
     */
    virtual void updateParticles() = 0;
};
