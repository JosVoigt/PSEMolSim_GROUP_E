#include "simulation/Simulation.h"

#include <utility>

#include "force/Force.h"
#include "simulation/StoermerVerlet.h"

// only undefine if output is not wished for
#define output_file

/**
 * \brief
 *  Construct a new Simulation object.
 *  This will act like a struct for all the required constants.
 *  Calling run() on the object will start the actual simulation
 * \param container
 *  The particle container containing all particles for the simulation.
 * \param method_
 *  The method to calculate the force.
 *  This will be used to calculate all forces between the particles in container.
 * \param writer_
 *  This provides the output method for the particle state.
 * \param dt_
 *  The time difference between each step.
 * \param outputFrequency_
 *  The frequency at which output will be generated.
 *  For every n-th iteration. (calculated by iteration % n)
 * \param filename_
 *  The prefix for the files that are going to get outputted.
*/
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

/**
 * \brief
 *  This will run the simulation.
 *  It will call all required operations to calculate Störmer-Verlet.
 *  The simulation object itself include all the constants that will be used.
 * \param start
 *  The start point at which output will be generated.
 *  The simulation still starts at t = 0.
 * \param end
 *  The end point of the simulation. This will be the last time step calculated.
 *  The function will return afterwards.
*/
void Simulation::run(double start, double end) {
    for (int iteration = 0; iteration <= end / dt; iteration++) {
        calculateX(container, dt, dt_sq);
        calculateF(container, method);
        calculateV(container, dt);

// if output_file at the top of the file is undefined, the compiler will not
// include this line
//
// intended for maximum speed, DO NOT USE while doing actual
// calculations
#ifdef output_file
        if (iteration >= start && ((iteration % outputFrequency) == 0)) {
            out->plotParticles(container, filename, iteration);
        }
#endif
    }
}
