#include "LenJonesForceMixed.h"

#include <spdlog/spdlog.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <set>
#include <tuple>
#include <utility>
#include <vector>

#include "container/Particle.h"
#include "utils/ArrayUtils.h"

LennardJonesForceMixed::LennardJonesForceMixed(std::vector<double> epsilon_,
                                               std::vector<double> sigma_,
                                               std::vector<int> type_) {
  if (epsilon.size() != sigma.size() && sigma.size() != type.size()) {
    spdlog::get("console")->critical(
        "Provided LenJones constant list length does not match up. \n"
        "Lengths were: epsilson {} sigma {} type {}",
        epsilon.size(), sigma.size(), type.size());
    exit(1);
  }

  epsilon = epsilon_;
  sigma = sigma_;
  type = type_;
}

std::pair<double, double> LennardJonesForceMixed::calculateMixingRule(
    double sig_a, double eps_a, double sig_b, double eps_b) {
  //already precalc the 24 for later
  double eps_mixed = std::sqrt(eps_a * eps_b);
  double sig_mixed = (sig_a + sig_b) / 2;

  return {sig_mixed, eps_mixed};
}

std::array<double, 3> LennardJonesForceMixed::calculateForce(
    Particle& p1, Particle& p2) const {
  // fetch types
  int type1 = p1.getType();
  int type2 = p2.getType();

  int pos1, pos2 = 0;

  for (int i = 0; i < type.size(); i++) {
    if (type[i] == type1) {
      pos1 = i;
    }
    if (type[i] == type2) {
      pos2 = i;
    }
  }

  std::pair<double, double> sig_eps = calculateMixingRule(
      sigma[pos1], epsilon[pos1], sigma[pos2], epsilon[pos2]);

  double sig = sig_eps.first;
  double eps = sig_eps.second;

  std::array<double, 3> vector = (p1.getX() - p2.getX());
  double distance = ArrayUtils::L2Norm(vector);
  double summand_6 = std::pow((sig / distance), 6);
  double summand_12 = std::pow((sig / distance), 12);

  std::array<double, 3> force = -24 * (eps / std::pow(distance, 2)) *
                                (summand_6 - (2 * summand_12)) * vector;

  return force;
}

std::string LennardJonesForceMixed::typeString() { return {}; }
