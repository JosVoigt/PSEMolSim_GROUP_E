#include "DiscGenerator.h"
#include "utils/ArrayUtils.h"
#include "utils/MaxwellBoltzmannDistribution.h"

#include <cmath>

DiscGenerator::DiscGenerator(int radius_, double distance_, double mass_,
                             std::array<double, 3> velocity_, std::array<double, 3> &center_)
    : radius(radius_), distance(distance_), mass(mass_), velocity(velocity_),
      center(center_){};

void DiscGenerator::readData(std::list<Particle> &list, int dimensions) {
  std::array<double, 3> squareUpperLeft = {(double)-radius, (double)-radius, 0}j;
  std::array<double, 3> upperLeftCorner = distance * squareUpperLeft + center;
  for (int i = 0; i < radius; i++) {
    for (int j = 0; j < radius; j++) {
      std::array<double, 3> currentParticlePostition = {(double)i, (double)j,
                                                        0};
      std::array<double, 3> particleCoordinates =
          distance * currentParticlePostition + upperLeftCorner;
	  double velo_norm = ArrayUtils::L2Norm(velocity);

      // check if particle is in circle
      if (ArrayUtils::L2Norm(particleCoordinates - center) <=
          (radius * distance)) {
        std::array<double, 3> distrVelocity =
            maxwellBoltzmannDistributedVelocity(velo_norm, dimensions) + velocity;
        list.emplace_back(particleCoordinates, distrVelocity, mass, 0);
      }
    }
  }
};
