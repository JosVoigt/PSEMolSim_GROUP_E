#pragma once

#include <list>
#include <vector>

#include "Particle.h"

class ParticleContainer {
   private:
    std::vector<Particle> particleArray;
    std::size_t length;

   public:
    ParticleContainer(std::size_t count, std::list<Particle>& init);
    ~ParticleContainer() = default;

    std::vector<Particle>::iterator begin() { return particleArray.begin(); }
    std::vector<Particle>::iterator end() { return particleArray.end(); }

    [[nodiscard]] std::size_t size() const;
};