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
    ~ParticleContainer() = default;

    std::vector<Particle>::iterator begin() { return particleArray.begin(); }
    std::vector<Particle>::iterator end() { return particleArray.end(); }

    int size() const;
};