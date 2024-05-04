#include <list>

#include "container/Particle.h"

/**
 * Interface for the input of data
 */
class Initializer {
   public:
    Initializer(){};
    virtual ~Initializer(){};
    virtual void readData(std::list<Particle>& list) = 0;
};