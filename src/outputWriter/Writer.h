#pragma once

#include <string>
#include <vector>

#include "container/ParticleContainerBasic.h"

class Writer {
   public:
    virtual ~Writer() = default;
    virtual void plotParticles(std::shared_ptr<ParticleContainerInterface> &container,
                               const std::string &filename, int iteration) = 0;
    virtual std::string typeString() = 0;
};
