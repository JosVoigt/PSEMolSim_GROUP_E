#include "force/PlanetForce.h"

#include <cmath>

#include "utils/ArrayUtils.h"

std::array<double, 3> PlanetForce::calculateForce(Particle& p1,
                                                  Particle& p2) const {
  double massProduct = p1.getM() * p2.getM();
  double distance = ArrayUtils::L2Norm(p2.getX() - p1.getX());
  std::array<double, 3> force =
      (massProduct / std::pow(distance, 3)) * (p2.getX() - p1.getX());
  return force;
}

std::string PlanetForce::typeString() { return "Planet"; }
