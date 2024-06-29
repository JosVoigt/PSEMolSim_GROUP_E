#include "MembraneLenJonesForce.h"

#include <cmath>

#include "particles/Particle.h"
#include "utils/ArrayUtils.h"

MembraneLenJonesForce::MembraneLenJonesForce(double bondDistance_,
                                             double bondStrength)
    : bondDistance(bondDistance_), k(bondStrength){};

std::array<double, 3> MembraneLenJonesForce::calculateForce(
    Particle& p1, Particle& p2) const {
  //calculate distance
  double distance = ArrayUtils::L2Norm(p1.getX() - p2.getX());

  if (distance < std::pow(2, 1 / 6) * (p1.getSigma() + p2.getSigma()) / 2) {
    return LennardJonesForce::calculateForce(p1, p2);
  } else if (p1.isDirectNeighbour(p2)) {
    return k * (distance - bondDistance) / distance * (p1.getX() + p2.getX());
  } else if (p1.isDiagonalNeighbour(p2)) {
    return k * (distance - bondDistance * std::sqrt(2)) / distance *
           (p1.getX() + p2.getX());
  } else {
    return {0, 0, 0};
  }
}
