#include "force/Planet.h"

#include <cmath>

#include "utils/ArrayUtils.h"

/**
 *  \brief
 *  Calculates the relative force between two particles as if they were planets
 *
 *  |F| = (m1*m2)/(d^2) \n
 *  d   :     Distance between the two particles \n
 *  m1  :     Mass of the first particles \n
 *  m2  :     Mass of the second particle
 *
 *  The gravitational constant G is not included for the sake of simplicity
 *
 *  \param forcePartner
 *  The other planet in the equation
 *
 *  \return
 *  The force vector with |F| as described above and pointing from this
 *  planet to the other one
 */
std::array<double, 3> Planet::calculateForce(Particle& p1, Particle& p2) const {
    double massProduct = p1.getM() * p2.getM();
    double distance = ArrayUtils::L2Norm(p2.getX() - p1.getX());
    std::array<double, 3> force =
        (massProduct / std::pow(distance, 3)) * (p2.getX() - p1.getX());
    return force;
}

std::string Planet::typeString() { return "Planet"; }