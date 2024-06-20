#include "StoermerVerlet.h"
#include <memory>
#include "container/ParticleContainerInterface.h"

void calculateX(std::vector<Particle>& particleList,
                const double dt, const double dt_sq) {
  for (Particle &p : particleList) {
    std::array<double, 3> res =
        (dt * p.getV()) + (dt_sq / (2 * p.getM())) * p.getF();
    p.addX(res);
  }
}

void calculateV(std::vector<Particle>& particleList,
                double dt) {
  for (Particle &p : particleList) {
    std::array<double, 3> res =
        (dt / (2 * p.getM())) * (p.getF() + p.getOldF());
    p.addV(res);
  }
}

void calculateF(std::vector<Particle>& particleList, std::shared_ptr<ParticleContainerInterface>& container,
                std::shared_ptr<PairwiseForce> method) {
  for (Particle &p : particleList) {
    p.nextIteration();
  }

  for (Particle &p : particleList) {
    std::vector<Particle> neighbors = container->retrieveRelevantParticles(p);

    for (Particle &neighbor : neighbors) {
      std::array<double, 3> f = method->calculateForce(p, neighbor);
      p.addF(f);
	  //newtons 3rd law
      std::array<double, 3> invF = -1 * f;
      neighbor.addF(invF);
    }
  }
}

void calculateGeneralF(std::vector<Particle> &particleList, std::shared_ptr<GeneralForce> &force) {
	for (auto& p : particleList) {
		auto particleForce = force->calculateForce(p);
		p.addF(particleForce);
	}
}
