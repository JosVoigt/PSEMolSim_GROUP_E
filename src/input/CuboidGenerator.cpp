#include "CuboidGenerator.h"

#include "utils/ArrayUtils.h"
#include "utils/MaxwellBoltzmannDistribution.h"

CuboidGenerator::CuboidGenerator(int x_, int y_, int z_, double distance,
                                 double mass_, double meanBrownMotion,
                                 std::array<double, 3> lowerLeftFrontCorner_,
                                 std::array<double, 3> initialVelocity_)
    : x(x_),
      y(y_),
      z(z_),
      mass(mass_),
      h(distance),
      meanBrownianMotion(meanBrownMotion),
      lowerLeftFrontCorner(lowerLeftFrontCorner_),
      initialVelocity(initialVelocity_) {}

void CuboidGenerator::readData(std::list<Particle>& list) {
    for (int X = 0; X < x; X++) {
        for (int Y = 0; Y < y; Y++) {
            for (int Z = 0; Z < z; Z++) {
                std::array<double, 3> location = {(double)X, (double)Y,
                                                  (double)Z};
                std::array<double, 3> position =
                    (h * location) + lowerLeftFrontCorner;
                std::array<double, 3> velocity =
                    initialVelocity +
                    maxwellBoltzmannDistributedVelocity(meanBrownianMotion, 3);
                list.emplace_back(position, velocity, mass);
            }
        }
    }
}