#pragma once

//Forward declartion of Particle for the compiler
class Particle;
#include <container/Particle.h>

#include <array>
#include <string>

/* Interface for the force calculation
 */
class PairwiseForce {
 public:
  /**
     *  \brief
     *  Destructor
     */
  virtual ~PairwiseForce() = default;

  /**
     *  \brief
     *  Calculates the force between two particles
     *
     *  \param p1
     *  The first particle
     *
     *  \param p2
     *  The second particle
     *
     *  \return
     *  The force vector between the two particles
     */
  virtual std::array<double, 3> calculateForce(Particle &p1,
                                               Particle &p2) const = 0;

  /** \brief
     *  Returns a string representation of the force
     *
     *  \return
     *  A string representation of the force
     */
  virtual std::string typeString() = 0;
};