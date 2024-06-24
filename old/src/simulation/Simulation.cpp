#include "simulation/Simulation.h"

#include <spdlog/spdlog.h>

#include <cmath>
#include <utility>
#include <vector>

#include "force/Force.h"
#include "simulation/StoermerVerlet.h"
#include "utils/MaxwellBoltzmannDistribution.h"

Simulation::Simulation(std::shared_ptr<ParticleContainerInterface> &container_,
                       std::shared_ptr<PairwiseForce> method_,
                       std::shared_ptr<Writer> writer_, double dt_,
                       int outputFrequency_, std::string filename_,
                       int dimensions_, std::shared_ptr<GeneralForce> general_)
    : container(std::move(container_)),
      method(std::move(method_)),
      out(std::move(writer_)),
      dt(dt_),
      outputFrequency(outputFrequency_),
      filename(std::move(filename_)),
      dimensions(dimensions_),
      general(general_) {
  dt_sq = std::pow(dt, 2);
}

void Simulation::initialize(double init_temperature) {
  for (auto p : container->preprocessParticles()) {
    auto particleMass = p.getM();

    //brownian motion factor
    double mean = std::sqrt(init_temperature / particleMass);

    //hacky set way
    p.scaleV(0);
    std::array<double, 3> mBD =
        maxwellBoltzmannDistributedVelocity(mean, dimensions);
    p.addV(mBD);
  }
}

void Simulation::run(double start, double end) {
#ifndef NO_LOG
  spdlog::get("file")->debug("Expected iterations: {}", (end / dt));
#endif
  for (int iteration = 0; iteration <= (end / dt); iteration++) {
    std::vector<Particle> allParticles = container->preprocessParticles();

    calculateX(allParticles, dt, dt_sq);
    calculateF(allParticles, container, method);
    if (general != nullptr) {
      calculateGeneralF(allParticles, general);
    }
    calculateV(allParticles, dt);

    // if NO_OUT_FILE is defined, the compiler will not
    // include this line
    //
    // intended for maximum speed or casual testing, DO NOT USE while doing
    // actual calculations
#ifndef NO_OUT_FILE
    if (iteration >= start / dt && ((iteration % outputFrequency) ==
                                    ((int)(start / dt) % outputFrequency))) {
      out->plotParticles(container, filename, iteration);
    }
#endif
  }
}
