#pragma once

#include <list>
#include <vector>

#include "Particle.h"

class ParticleContainer {
   private:
    std::vector<Particle> particleArray;
    int length;

   public:
    ParticleContainer(int count, std::list<Particle>& init);
    ~ParticleContainer();

    auto begin() { return particleArray.begin(); }
    auto end() { return particleArray.end(); }

    int size() const;
    Particle& getParticleAt(int idx);
};