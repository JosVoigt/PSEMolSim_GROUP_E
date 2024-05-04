#include "simulation/Simulation.h"

#include <iostream>

#include "force/Force.h"
#include "simulation/StoermerVerlet.h"

Simulation::Simulation(ParticleContainer& container_, Force* method_,
                       Writer* writer_, double dt_, int outputFrequency_,
                       std::string filename_)
    : container(container_),
      method(method_),
      out(writer_),
      dt(dt_),
      outputFreqency(outputFrequency_),
      filename(filename_) {
    dt_sq = std::pow(dt, 2);
}

void Simulation::run(double start, double end) {
    for (int iteration = 0; iteration <= end; iteration++) {
        calculateX(container, dt, dt_sq);
        calculateF(container, method);
        calculateV(container, dt);

        if (iteration >= start && iteration % outputFreqency == 0) {
            out->plotParticles(container, filename, iteration);
        }
    }
}
