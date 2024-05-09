#include "simulation/Simulation.h"

#include <utility>

#include "force/Force.h"
#include "simulation/StoermerVerlet.h"

Simulation::Simulation(ParticleContainer& container_,
                       std::shared_ptr<Force> method_,
                       std::shared_ptr<Writer> writer_, double dt_,
                       int outputFrequency_, std::string filename_)
    : container(container_),
      method(method_),
      out(writer_),
      dt(dt_),
      outputFrequency(outputFrequency_),
      filename(std::move(filename_)) {
    dt_sq = std::pow(dt, 2);
}

void Simulation::run(double start, double end) {
    for (int iteration = 0; iteration <= end / dt; iteration++) {
        calculateX(container, dt, dt_sq);
        calculateF(container, method);
        calculateV(container, dt);

        if (iteration >= start && ((iteration % outputFrequency) == 0)) {
            out->plotParticles(container, filename, iteration);
        }
    }
}
