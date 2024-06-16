#pragma once

#include <limits>
#include <memory>
#include <string>

#include "container/ParticleContainerBasic.h"
#include "force/Force.h"
#include "outputWriter/Writer.h"

class Simulation {
 private:
  /**
   * \brief
   *  The container containing all particles for the simulation.
   */
  std::shared_ptr<ParticleContainerInterface> container;

  /**
   * \brief
   *  The method to calculate the force.
   *  This will be used to calculate all forces between the particles in
   * container.
   */
  std::shared_ptr<PairwiseForce> method;

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

  std::array<int, 3> linkedCellDimensions;
  double linkedCellSidelength;

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
   *  This will be used to calculate all forces between the particles in
   * container. \param writer_ This provides the output method for the particle
   * state. \param dt_ The time difference between each step. \param
   * outputFrequency_ The frequency at which output will be generated. For every
   * n-th iteration. (calculated by iteration % n) \param linkedCellDimensions
   *    The amount of cubes in the space used for linked cells given as {x,y,z}.
   *    Defaults to {1,1,1}.
   * \param linkedCellSidelength
   *    Sets the cube side length for the linkedCell algortihm
   *    To deactivate set to double:infinity
   *    Will exit on values <= 0
   * \param filename_
   *  The prefix for the files that are going to get outputted.
   */
  Simulation(
      std::shared_ptr<ParticleContainerInterface> &container_,
      std::shared_ptr<PairwiseForce> method_, std::shared_ptr<Writer> writer_,
      double dt_, int outputFrequency, std::string filename_,
      std::array<int, 3> linkedCellDimensions = {1, 1, 1},
      double linkedCellSidelength = std::numeric_limits<double>::infinity());

  /**
   * \brief
   *  This will run the simulation.
   *  It will call all required operations to calculate Störmer-Verlet.
   *  The simulation object itself include all the constants that will be used.
   * \param start
   *  The start point at which output will be generated.
   *  The simulation still starts at t = 0.
   * \param end
   *  The end point of the simulation. This will be the last time step
   * calculated. The function will return afterwards.
   */
  void run(double start, double end);
};
