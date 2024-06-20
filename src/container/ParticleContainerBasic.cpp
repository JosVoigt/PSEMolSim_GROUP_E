#include "ParticleContainerBasic.h"

ParticleContainer::ParticleContainer(std::size_t length_,
                                     std::list<Particle>& init)
    : length(length_),
      particleArray(std::begin(init), std::end(init)){}

std::size_t ParticleContainer::size() const { return length; }


std::vector<Particle> ParticleContainer::preprocessParticles() {
  return particleArray;
}

std::vector<Particle> ParticleContainer::retrieveRelevantParticles(
    Particle& particle) {
  return particleArray;
}
void ParticleContainer::updateParticles() {}

int getIndexFromCoordinates(std::array<int, 3> cellCoordinates) { return -1; }

void ParticleContainer::insertParticle(const Particle& particle) {
  particleArray.push_back(particle);
}
