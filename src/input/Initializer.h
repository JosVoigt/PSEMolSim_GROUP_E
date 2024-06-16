#pragma once

#include <list>

#include "container/Particle.h"

/**
 * Interface for the input of data
 */
class Initializer {
 public:
  Initializer() = default;
  virtual ~Initializer() = default;
  virtual void readData(std::list<Particle>& list, int dimensions) = 0;
};
