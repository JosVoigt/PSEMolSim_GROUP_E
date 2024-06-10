/*
 * Particle.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

//Force predecleration for the compiler
class PairwiseForce;
#include "force/Force.h"

#include <array>
#include <memory>
#include <string>

/**
 * \brief
 *  Represents a particle in the simulation
 */
class Particle {
private:
  /**
   * Position of the particle
   */
  std::array<double, 3> x{};

  /**
   * Velocity of the particle
   */
  std::array<double, 3> v{};

  /**
   * Force effective on this particle
   */
  std::array<double, 3> f{};

  /**
   * Force which was effective on this particle
   */
  std::array<double, 3> old_f{};

  /**
   * Mass of this particle
   */
  double m{};

  /**
   * Type of the particle. Use it for whatever you want (e.g. to separate
   * molecules belonging to different bodies, matters, and so on)
   */
  int type;

  /**
   * Abstraction to calculate the force for the particles
   */
  std::shared_ptr<PairwiseForce> forcemode;

public:
  /**
   * \brief
   *  Default constructor
   */
  explicit Particle(int type = 0);

  /**
   * \brief
   *  Copy constructor
   * \param other
   *  The particle to be copied
   */
  Particle(const Particle &other);

  /**
   * \brief
   *  Constructor
   * \param x_arg
   *  The position of the particle
   * \param v_arg
   *  The velocity of the particle
   * \param m_arg
   *  The mass of the particle
   * \param type
   *  The type of the particle
   */
  Particle(
      // for visualization, we always need 3 coordinates
      // -> in case of 2d, we use only the first and the second
      std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg,
      int type = 0);

  /**
   * \brief
   *  Destructor
   */
  ~Particle();

  // Getter
  /**
   * \brief
   *  Returns the position of the particle
   * \return
   *  The position of the particle
   */
  [[nodiscard]] const std::array<double, 3> &getX() const;

  /**
   * \brief
   *  Returns the velocity of the particle
   * \return
   *  The velocity of the particle
   */
  [[nodiscard]] const std::array<double, 3> &getV() const;

  /**
   * \brief
   *  Returns the force of the particle
   * \return
   *  The force of the particle
   */
  [[nodiscard]] const std::array<double, 3> &getF() const;

  /**
   * \brief
   *  Returns the old force of the particle
   * \return
   *  The old force of the particle
   */
  [[nodiscard]] const std::array<double, 3> &getOldF() const;

  /**
   * \brief
   *  Returns the mass of the particle
   * \return
   *  The mass of the particle
   */
  [[nodiscard]] double getM() const;

  /**
   * \brief
   *  Returns the type of the particle
   * \return
   *  The type of the particle
   */
  [[nodiscard]] int getType() const;
  // End Getter

  /**
   * \brief
   *  Adds the given force to the current force
   * \param aF
   *  The force to be added
   */
  void addF(std::array<double, 3> &aF);

  /**
   * \brief
   *  Adds the given position to the current position
   * \param aX
   *  The position to be added
   */
  void addX(std::array<double, 3> &aX);

  /**
   * \brief
   *  Adds the given speed to the current speed
   * \param aV
   *  The speed to be added
   */
  void addV(std::array<double, 3> &aV);

  /**
   * \brief
   * Scales the velocity according to the provided scaling factor
   * \param scale
   * The scaling factor
   */
  void scaleV(double scale);

  /**
   * \brief
   *  Prepares the particles for the next iterations by making the current the
   *  old force and setting the current force to {0,0,0}
   */
  void nextIteration();

  /**
   * \brief
   *  Compares two particles
   * \param other
   *  The particle to be compared with
   * \return
   *  True if the particles are equal, false otherwise
   */
  bool operator==(const Particle &other) const;

  /**
   * \brief
   *  Converts the particle to a string
   * \return
   *  The particle as a string
   */
  [[nodiscard]] std::string toString() const;
};

/**
 * \brief
 *  Prints the particle to the given stream
 * \param stream
 *  The stream to print to
 * \param p
 *  The particle to be printed
 * \return
 *  The stream
 */
std::ostream &operator<<(std::ostream &stream, Particle &p);
