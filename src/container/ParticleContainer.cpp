#include "ParticleContainer.h"

ParticleContainer::ParticleContainer(int length_, std::list<Particle>& init)
    : length(length_) {
    // init new array
    particleArray = std::vector<Particle>();
    particleArray.reserve(length);

    for (Particle& p : init) {
        particleArray.push_back(p);
    }
}

int ParticleContainer::size() const { return length; }