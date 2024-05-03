#pragma once

#include <string>
#include <vector>

#include "container/ParticleContainer.h"

class Writer {
   public:
    virtual void plotParticles(ParticleContainer &container,
                               const std::string &filename, int iteration) = 0;
};
