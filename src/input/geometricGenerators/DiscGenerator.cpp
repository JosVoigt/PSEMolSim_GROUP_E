#include "DiscGenerator.h"
#include "../../src/utils/ArrayUtils.h"
#include "../../src/utils/MaxwellBoltzmannDistribution.h"

#include <cmath>

DiscGenerator::DiscGenerator(int radius_, double distance_, double mass_,
                             double velocity_, std::array<double, 3> &center_)
    : radius(radius_), distance(distance_), mass(mass_), velocity(velocity_),
      center(center_){};

void DiscGenerator::readData(std::list<Particle> &list) {
  std::array<double, 3> squareUpperLeft = {(double)-radius, (double)-radius, 0};
  std::array<double, 3> upperLeftCorner = distance * squareUpperLeft + center;
  for (int i = 0; i < radius; i++) {
    for (int j = 0; j < radius; j++) {
      std::array<double, 3> currentParticlePostition = {(double)i, (double)j,
                                                        0};
      std::array<double, 3> particleCoordinates =
          distance * currentParticlePostition + upperLeftCorner;

      // check if particle is in circle
      if (ArrayUtils::L2Norm(particleCoordinates - center) <=
          (radius * distance)) {
        std::array<double, 3> distrVelocity =
            maxwellBoltzmannDistributedVelocity(velocity, 3);
        list.emplace_back(particleCoordinates, distrVelocity, mass, 0);
      }
    }
  }
};
