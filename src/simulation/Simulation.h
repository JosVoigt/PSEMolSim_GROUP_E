#pragma once

#include <memory>
#include <string>

#include "container/ParticleContainer.h"
#include "force/Force.h"
#include "outputWriter/Writer.h"

class Simulation {
   private:
    /**
     * \brief
     *  The container containing all particles for the simulation.
     */
    ParticleContainer container;

    /**
     * \brief
     *  The method to calculate the force.
     *  This will be used to calculate all forces between the particles in container.
     */
    std::shared_ptr<Force> method;

    /**
     * \brief
     *  This provides the output method for the particle state.
     */
    std::shared_ptr<Writer> out;

    /**
     * \brief
     *  The time difference between each step.
     */
    double dt;

    /**
     * \brief
     *  The time difference dt squared.
     *  If dt_sq is not equal to dt^2 behaviour is undefined.
     */
    double dt_sq;

    /**
     * \brief
     *  The frequency at which output will be generated.
     *  For every n-th iteration. (calculated by iteration % n)
     */
    int outputFrequency;

    /**
     * \brief
     *  The prefix for the files that are going to get outputted.
     */
    std::string filename;

   public:
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
    Simulation(ParticleContainer& container_, std::shared_ptr<Force> method_,
               std::shared_ptr<Writer> writer_, double dt_, int outputFrequency,
               std::string filename_);

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
    void run(double start, double end);
};