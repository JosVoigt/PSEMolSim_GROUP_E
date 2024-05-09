#pragma once

#include <string>
#include <vector>

#include "Particle.h"

class writer {
   public:
    virtual void plotParticles(std::vector<Particle *> &particles,
                               const std::string &filename, int iteration) {
        return;
    };
};
