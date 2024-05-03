/*
 * Particle.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include <array>
#include <string>

class Particle {
   private:
    /**
     * Position of the particle
     */
    std::array<double, 3> x;

    /**
     * Velocity of the particle
     */
    std::array<double, 3> v;

    /**
     * Force effective on this particle
     */
    std::array<double, 3> f;

    /**
     * Force which was effective on this particle
     */
    std::array<double, 3> old_f;

    /**
     * Mass of this particle
     */
    double m;

    /**
     * Type of the particle. Use it for whatever you want (e.g. to separate
     * molecules belonging to different bodies, matters, and so on)
     */
    int type;

   public:
    explicit Particle(int type = 0);

    Particle(const Particle& other);

    Particle(
        // for visualization, we need always 3 coordinates
        // -> in case of 2d, we use only the first and the second
        std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg,
        int type = 0);

    ~Particle();

    // Getter
    const std::array<double, 3>& getX() const;
    const std::array<double, 3>& getV() const;
    const std::array<double, 3>& getF() const;
    const std::array<double, 3>& getOldF() const;
    double getM() const;
    int getType() const;
    // End Getter

    /**
     * \brief
     *  Adds the given force to the current force
     * \param aF
     *  The force to be added
     */
    void addF(std::array<double, 3>& aF);
    /**
     * \brief
     *  Adds the given position to the current position
     * \param aX
     *  The position to be added
     */
    void addX(std::array<double, 3>& aX);
    /**
     * \brief
     *  Adds the given speed to the current speed
     * \param aV
     *  The speed to be added
     */
    void addV(std::array<double, 3>& aV);

    /**
     * \brief
     *  Prepares the particles for the next iterations by making the current the
     *  old force and setting the current force to {0,0,0}
     */
    void nextIteration();

    bool operator==(Particle& other);
    std::string toString() const;
};

std::ostream& operator<<(std::ostream& stream, Particle& p);
