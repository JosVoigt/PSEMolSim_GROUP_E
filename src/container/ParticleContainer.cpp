#include "ParticleContainer.h"

ParticleContainer::ParticleContainer(std::size_t length_,
                                     std::list<Particle>& init)
    : length(length_) {
    // init new array
    particleArray = std::vector<Particle>();
    particleArray.reserve(length);

    for (Particle& p : init) {
        particleArray.push_back(p);
    }
}

/**
 * \brief
 *  Returns the amount of particles in the container.
 * \return
 *  The amount of particles in the container
 */
std::size_t ParticleContainer::size() const { return length; }