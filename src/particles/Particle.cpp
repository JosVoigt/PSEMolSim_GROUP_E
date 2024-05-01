/*
 * Particle.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include "Particle.h"

#include <cmath>
#include <iostream>

#include "utils/ArrayUtils.h"

Particle::Particle(int type_arg) {
    type = type_arg;
    std::cout << "Particle generated!" << std::endl;
    f = {0., 0., 0.};
    old_f = {0., 0., 0.};
    x = {0, 0, 0};
    v = {0, 0, 0};
}

Particle::Particle(const Particle& other) {
    x = other.x;
    v = other.v;
    f = other.f;
    old_f = other.old_f;
    m = other.m;
    type = other.type;
    std::cout << "Particle generated by copy!" << std::endl;
}

Particle::Particle(std::array<double, 3> x_arg, std::array<double, 3> v_arg,
                   double m_arg, int type_arg)
    : x(x_arg),
      v(v_arg),
      f({0., 0., 0.}),
      old_f({0., 0., 0.}),
      m(m_arg),
      type(type_arg) {
    std::cout << "Particle generated!" << std::endl;
}

Particle::~Particle() { std::cout << "Particle destructed!" << std::endl; }

// Getter
const std::array<double, 3>& Particle::getX() const { return x; }
const std::array<double, 3>& Particle::getV() const { return v; }
const std::array<double, 3>& Particle::getF() const { return f; }
const std::array<double, 3>& Particle::getOldF() const { return old_f; }
double Particle::getM() const { return m; }
int Particle::getType() const { return type; }

/**
 * \brief
 *   adds a force to the total force in the particle
 *
 * \param aF
 *   The force to be added (a 3D vector)
 */
const void Particle::addF(std::array<double, 3>& aF) {
    for (int i = 0; i < 3; i++) {
        f[i] += aF[i];
    }
}
/**
 * \brief
 *   Prepare the single particle for the next iteration
 *   The current force becomes the old force and the current force is set to 0
 *
 */
const void Particle::nextIteration() {
    old_f = f;
    f = {0, 0, 0};
}

/**
 * \brief
 *   Calculates distance between this and the passed particle
 *
 * \param p
 *   The other particle the distance is to be calculated with
 */
double Particle::distance(Particle& p) {
    std::array<double, 3> pos_other = p.getX();

    double result = 0;

    for (int i = 0; i < 3; i++) {
        double dim_dist = x[i] - pos_other[i];
        result += dim_dist * dim_dist;
    }

    result = std::sqrt(result);

    return result;
}

/**
 * \brief
 *   Calculates the vector that points from this particles position to the other
 *   particles position
 * \param p
 *  The other particle in the calculation
 * \return
 *  The vector that points from this particle to p
 */
std::array<double, 3> Particle::positionVector(Particle& p) {
    std::array<double, 3> result;

    for (int i = 0; i < 3; i++) {
        result[i] = x[i] - p.getX()[i];
    }

    return result;
}

/**
 * \brief
 *   Calculates the new position vector based on Störmer-Verlet
 * \param dt
 *   The time between steps
 */
void Particle::calculateX(double dt) {
    for (int i = 0; i < 3; i++) {
        x[i] = x[i] + dt * v[i] + std::pow(dt, 2) * old_f[i] / (2 * m);
    }
}

/**
 * \brief
 *   Calculates the new velocity vector based on Störmer-Verlet
 * \param dt
 *   The time between steps
 */
void Particle::calculateV(double dt) {
    for (int i = 0; i < 3; i++) {
        v[i] = v[i] + dt * (f[i] + old_f[i]) / (2 * m);
    }
}

std::string Particle::toString() const {
    std::stringstream stream;
    stream << "Particle: X:" << x << " v: " << v << " f: " << f
           << " old_f: " << old_f << " type: " << type;
    return stream.str();
}

bool Particle::operator==(Particle& other) {
    return (x == other.x) and (v == other.v) and (f == other.f) and
           (type == other.type) and (m == other.m) and (old_f == other.old_f);
}

std::ostream& operator<<(std::ostream& stream, Particle& p) {
    stream << p.toString();
    return stream;
}