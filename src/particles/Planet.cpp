#include "header/Planet.h"

#include <cmath>

Planet::Planet(Particle& p) : Particle(p){};

/**
 * \brief
 *  Calculates the relative force between two particles as if they were planets
 *  |F| = (m1*m2)/(d^2)
 *  The gravitational constant G is not included for the sake of simplicity
 * \param forcePartner
 *  The other planet in the equation
 * \return The force vector with |F| as described above and pointing from this
 *      planet to the other one
 */
std::array<double, 3> Planet::calculateForce(Particle& forcePartner) {
    double massProduct = getM() * forcePartner.getM();
    double distanceCubed = std::pow(distance(forcePartner), 3);
    std::array<double, 3> forceDirectionVector = positionVector(forcePartner);

    std::array<double, 3> forceVector;

    for (int i = 0; i < 3; i++) {
        forceVector[i] = forceDirectionVector[i] * massProduct / distanceCubed;
    }

    return forceVector;
}