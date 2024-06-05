#include <vector>

#include "Particle.h"

class ParticleContainerInterface {
 public:
  virtual std::vector<Particle> preprocessParticles();
  virtual std::vector<Particle> getParticles(Particle& p);
  virtual void updateParticles();
};
