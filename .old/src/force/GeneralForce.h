#include "container/Particle.h"

class GeneralForce {
 public:
  virtual ~GeneralForce() = default;

  virtual std::array<double, 3> calculateForce(Particle& p) = 0;
};
