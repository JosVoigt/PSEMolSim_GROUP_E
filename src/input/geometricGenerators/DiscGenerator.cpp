#include "DiscGenerator.h"

#include <cmath>

DiscGenerator::DiscGenerator(double radius_, double mass_, std::array<double, 3>& center_)
    : radius(radius_),
      mass(mass_),
      center(center_){};

void DiscGenerator::readData(std::list<Particle>& list) {
    double r_sq = std::pow(radius, 2);
};