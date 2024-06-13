#include "StoermerVerlet.h"

void calculateX(std::shared_ptr<ParticleContainerInterface> &container,
                const double dt, const double dt_sq) {

  std::vector<Particle> allParticles = container->preprocessParticles();

  for (Particle &p : allParticles) {
    std::array<double, 3> res =
        (dt * p.getV()) + (dt_sq / (2 * p.getM())) * p.getF();
    p.addX(res);
  }
}

void calculateV(std::shared_ptr<ParticleContainerInterface> &container,
                double dt) {

  std::vector<Particle> allParticles = container->preprocessParticles();

  for (Particle &p : allParticles) {
    std::array<double, 3> res =
        (dt / (2 * p.getM())) * (p.getF() + p.getOldF());
    p.addV(res);
  }
}

void calculateF(std::shared_ptr<ParticleContainerInterface> &container,
                std::shared_ptr<PairwiseForce> method) {

  std::vector<Particle> allParticles = container->preprocessParticles();

  for (Particle &p : allParticles) {
    p.nextIteration();
  }

  for (Particle &p : allParticles) {

    std::vector<Particle> neighbors = container->retrieveRelevantParticles(p);

    for (Particle &neighbor : neighbors) {
      std::array<double, 3> f = method->calculateForce(p, neighbor);
      p.addF(f);
      std::array<double, 3> invF = -1 * f;
      neighbor.addF(invF);
    }
  }
}
