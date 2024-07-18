#pragma once

#include <array>

#include "input/Initializer.h"

class CuboidGenerator : public Initializer {
 private:
  /**
   * \brief
   *  The dimensions of the cuboid in x-direction
   */
  int x;

  /**
   * \brief
   *  The dimensions of the cuboid in y-direction
   */
  int y;

  /**
   * \brief
   *  The dimensions of the cuboid in z-direction
   */
  int z;

  /**
   * \brief
   *  The distance between the particles in every direction
   */
  double h;

  /**
   * \brief
   *  The mass of every particle in the cuboid
   */
  double mass;

  /**
   * \brief
   *  The mean brownian motion of the particles in the cuboid.
   */
  double meanBrownianMotion;

  /**
   * \brief
   *  The 3D coordinate of the corner with minimal coordinates (lower, left,
   * front)
   */
  std::array<double, 3> lowerLeftFrontCorner;

  /**
   * \brief
   *  The average initial velocity of all the particles in the cuboid
   */
  std::array<double, 3> initialVelocity;

  /**
   * \brief
   * The type of the particle
   */
  int type;

 public:
  /**
   * \brief
   *  Holds all the necessary information to construct the particle cuboid.
   * \param x_
   *  Dimension in x-direction
   * \param y_
   *  Dimension in y-direction
   * \param z_
   *  Dimension in z-direction
   * \param distance
   *  The distance between the particles in every direction
   * \param mass_
   *  The mass of every particle in the cuboid
   * \param meanBrownMotion
   *  The mean brownian motion of the particles in the cuboid.
   * \param lowerLeftFrontCorner_
   *  The 3D coordinate of the corner with minimal coordinates (lower, left,
   * front)
   * \param initialVelocity_ The average initial velocity of all the
   * particles in the cuboid
   */
  CuboidGenerator(int x_, int y_, int z_, double distance, double mass_,
                  double meanBrownMotion,
				  int type,
                  std::array<double, 3> lowerLeftFrontCorner_,
                  std::array<double, 3> initialVelocity_);

  /**
   * \brief
   *  Destructor
   */
  ~CuboidGenerator() override = default;

  /**
   * \brief
   *  Generates the particles in a cuboid.
   *  Dimensions are passed via the constructor.
   * \param list
   *  The list that will hold the generated particles.
   *  The generated particles will be appended to the end of the list.
   *  The list can be empty or already containing particles.
   * \param dimensions
   *	The dimensions, required to initialize the brownian motion
   */
  void readData(std::list<Particle> &list, int dimensions) override;

  /**
   * \brief
   *  Getter for the x dimension of the cuboid
   * \return
   *  The x dimension of the cuboid
   */
  [[nodiscard]] int getX() const;

  /**
   * \brief
   *  Getter for the y dimension of the cuboid
   * \return
   *  The y dimension of the cuboid
   */
  [[nodiscard]] int getY() const;

  /**
   * \brief
   *  Getter for the z dimension of the cuboid
   * \return
   *  The z dimension of the cuboid
   */
  [[nodiscard]] int getZ() const;

  /**
   * \brief
   *  Getter for the distance between the particles in every direction
   * \return
   *  The distance between the particles in every direction
   */
  [[nodiscard]] double getH() const;

  /**
   * \brief
   *  Getter for the mass of every particle in the cuboid
   * \return
   *  The mass of every particle in the cuboid
   */
  [[nodiscard]] double getMass() const;

  /**
   * \brief
   *  Getter for the mean brownian motion of the particles in the cuboid.
   * \return
   *  The mean brownian motion of the particles in the cuboid.
   */
  [[nodiscard]] double getMeanBrownianMotion() const;

  /**
   * \brief
   *  Getter for the 3D coordinate of the corner with minimal coordinates
   * (lower, left, front)
   * \return
   *  The 3D coordinate of the corner with minimal coordinates (lower, left,
   * front)
   */
  [[nodiscard]] const std::array<double, 3> &getLLFC() const;

  /**
   * \brief
   *  Getter for the average initial velocity of all the particles in the
   * cuboid
   * \return
   *  The average initial velocity of all the particles in the cuboid
   */
  [[nodiscard]] const std::array<double, 3> &getInitialVelocity() const;

    /**
     * \brief
     *  Getter for the type of the particle
     * \return
     *  The type of the particle
     */
    [[nodiscard]] int getType() const;
};
