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

    typedef std::vector<Particle>::iterator Iterator;

    Iterator begin() { return particleArray.begin(); }
    Iterator end() { return particleArray.end(); }

    [[nodiscard]] std::size_t size() const;
};