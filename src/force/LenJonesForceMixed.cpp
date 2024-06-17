#include "LenJonesForceMixed.h"Cuboi

#include <spdlog/spdlog.h>

#include <array>
#include <cmath>
#include <utility>
#include <vector>

#include "container/Particle.h"
#include "utils/ArrayUtils.h"

LennardJonesForceMixed::LennardJonesForceMixed(std::vector<double> epsilon,
                                               std::vector<double> sigma,
                                               std::vector<int> type) {
  if (epsilon.size() != sigma.size() != type.size()) {
    spdlog::get("console")->critical(
        "Provided LenJones constant list length does not match up. \n"
        "Lengths were: epsilson {} sigma {} type{}",
        epsilon.size(), sigma.size(), type.size());
    exit(1);
  }

  while (type.size() != 0) {
    // find lowest element
    int min_idx = 0;
    int min = type[0];
    for (int i = 0; i < type.size(); i++) {
      if (min > type[i]) {
        min_idx = i;
        min = type[i];
      }
    }

    // insert all min_idx combinations
    for (int i = 0; i < type.size(); i++) {
      int hashpos = z_curve_interleave(i, min);
      force_constants.insert_or_assign(
          hashpos, calculateMixingRule(sigma[i], epsilon[i], sigma[min_idx],
                                       epsilon[min_idx]));
    }

    // remove from the three lists
    epsilon.erase(epsilon.begin() + min_idx);
    sigma.erase(sigma.begin() + min_idx);
    type.erase(type.begin() + min_idx);
  }
};

int inline LennardJonesForceMixed::z_curve_interleave(int a, int b) const {
  // interleave with 0
  a *= a;
  b *= b;

  // bitshift one by 1
  b <<= 1;

  // interleave
  return (a | b);
}

std::pair<double, double> LennardJonesForceMixed::calculateMixingRule(
    double sig_a, double eps_a, double sig_b, double eps_b) {
  //already precalc the 24 for later
  double eps_mixed = 24 * std::sqrt(eps_a * eps_b);
  double sig_mixed = (sig_a + sig_b) / 2;

  return std::pair(sig_mixed, eps_mixed);
}

std::array<double, 3> LennardJonesForceMixed::calculateForce(
    Particle& p1, Particle& p2) const {
  // fetch types
  int type1 = p1.getType();
  int type2 = p2.getType();

  int position;
  // interleave
  if (type1 > type2) {
    position = z_curve_interleave(type1, type2);
  } else {
    position = z_curve_interleave(type2, type1);
  }

  // fetch data from hashmap
  std::pair<double, double> sig_eps = force_constants.find(position)->second;
  double sig = sig_eps.first;
  double eps = sig_eps.second;

  // calculate distance
  std::array<double, 3> x1 = p1.getX();
  std::array<double, 3> x2 = p2.getX();
  std::array<double, 3> vector1_2 = x1 - x2;
  double distance = ArrayUtils::L2Norm(vector1_2);

  // lenjones
  double sig_dist_6 = std::pow(sig / distance, 6);
  std::array<double, 3> force = -eps / std::pow(distance, 2) *
                                (sig_dist_6 - (2 * std::pow(sig_dist_6, 2))) *
                                vector1_2;

  return force;
}
