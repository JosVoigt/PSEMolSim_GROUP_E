#pragma once

#include <array>

#include "Initializer.h"

class CuboidGenerator : public Initializer {
   private:
    int x;
    int y;
    int z;
    double h;
    double mass;
    double meanBrownianMotion;
    std::array<double, 3> lowerLeftFrontCorner;
    std::array<double, 3> initialVelocity;

   public:
    /**
     * \brief
     *  Holds all the necessary information to construct the particle cubiod.
     * \param x_
     *  Dimension in x-direction
     * \param y_
     *  Dimension in y-direction
     * \param z_
     *  Dimension in z-direction
     * \param distance
     *  The distance between the particles in every direction
     * \param mass_
     *  The mass of every particle in the cubiod
     * \param meanBrownMotion
     *  The mean brownian motion of the particles in the cuboid.
     * \param lowerLeftFrontCorner_
     *  The 3D coordinate of the corner with minimal coordinates (lower, left,
     * front) \param initialVelocity_ The average initial velocity of all the
     * particles in the cuboid
     */
    CuboidGenerator(int x_, int y_, int z_, double distance, double mass_,
                    double meanBrownMotion,
                    std::array<double, 3> lowerLeftFrontCorner_,
                    std::array<double, 3> initialVelocity_);

    ~CuboidGenerator() override = default;

    /**
     * \brief
     *  Generates the particles in a cuboid.
     *  Dimensions are passed via the constructor.
     * \param list
     *  The list that will hold the generated particles.
     *  The generated particles will be appended to the end of the list.
     *  The list can be empty or already containing particles.
     */
    void readData(std::list<Particle>& list) override;
};
