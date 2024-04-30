#include "header/Planet.h"

#include <cmath>

std::array<double, 3> Planet::calculateForce(Particle& forcePartner) {
    double massProduct = m * forcePartner.getM();
    double distanceCubed = std::pow(distance(forcePartner), 3);
    std::array<double, 3> forceDirectionVector = positionVector(forcePartner);

    std::array<double, 3> forceVector;

    for (int i = 0; i < 3; i++) {
        forceVector[i] = forceDirectionVector[i] * massProduct / distanceCubed;
    }

    return forceVector;
}