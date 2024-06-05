#include "ParticleContainerBasic.h"

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

std::size_t ParticleContainer::size() const { return length; }

std::vector<Particle> ParticleContainer::preprocessParticles(){
	return particleArray;
}
std::vector<Particle> ParticleContainer::insertIfRelevantParticles(Particle& p){
	return particleArray;
}
void ParticleContainer::updateParticles() {
	return;
}
