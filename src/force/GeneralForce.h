#include "container/Particle.h"

class GeneralForce {
 public:
  virtual ~GeneralForce();

  virtual std::array<double, 3> calculateForce(Particle& p);
};
