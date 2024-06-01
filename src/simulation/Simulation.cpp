#include "simulation/Simulation.h"

#include <spdlog/spdlog.h>

#include <utility>

#include "force/Force.h"
#include "simulation/StoermerVerlet.h"

Simulation::Simulation(ParticleContainer &container_,
                       std::shared_ptr<Force> method_,
                       std::shared_ptr<Writer> writer_, double dt_,
                       int outputFrequency_, std::string filename_,
                       std::array<int, 3> linkedCellDimensions,
                       double linkedCellSidelength)
    : container(container_),
      method(std::move(method_)),
      out(std::move(writer_)),
      dt(dt_),
      outputFrequency(outputFrequency_),
      filename(std::move(filename_)),
      linkedCellDimensions(std::move(linkedCellDimensions)),
      linkedCellSidelength(linkedCellSidelength) {
  dt_sq = std::pow(dt, 2);
}

void Simulation::run(double start, double end) {
#ifndef NO_LOG
  spdlog::get("file")->debug("Expected iterations: {}", (end / dt));
#endif
  for (int iteration = 0; iteration <= (end / dt); iteration++) {
    calculateX(container, dt, dt_sq);
    calculateF(container, method);
    calculateV(container, dt);

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
