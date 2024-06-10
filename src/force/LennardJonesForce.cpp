#include "LennardJonesForce.h"

#include <spdlog/spdlog.h>

#include <cmath>

#include "utils/ArrayUtils.h"

LennardJonesForce::LennardJonesForce(double epsilon_, double sigma_)
    : epsilon(epsilon_), sigma(sigma_) {
  epsilon_24 = epsilon * (-24);
}

std::array<double, 3> LennardJonesForce::calculateForce(Particle& p1,
                                                           Particle& p2) const {
  double distance = ArrayUtils::L2Norm(p1.getX() - p2.getX());

  double summand_6 = std::pow((sigma / distance), 6);
  double summand_12 = std::pow(summand_6, 2);

  std::array<double, 3> force = (epsilon_24 / std::pow(distance, 2)) *
                                (summand_6 - (2 * summand_12)) *
                                (p1.getX() - p2.getX());

#ifndef NO_LOG
  spdlog::get("file")->trace(
      "Calculating force LenJonesMol: s_6: {}, s_12: {}, dist: {}, force: {}",
      summand_6, summand_12, distance, ArrayUtils::to_string(force));
#endif

  return force;
}

std::string LennardJonesForce::typeString() {
  return "Lennard-Jones-Potential for molecules";
}
