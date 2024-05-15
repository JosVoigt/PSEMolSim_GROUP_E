#include "CuboidGenerator.h"

#include <spdlog/spdlog.h>

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

void CuboidGenerator::readData(std::list<Particle> &list) {
    if (x < 1 || y < 1 || z < 1) {
        spdlog::get("console")->error(
            "At least one dimension of a Cuboid was less than 0."
            "Will ignore this cuboid.");
        return;
    }

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
                spdlog::get("file")->debug("Particle emplaced back with X: " + ArrayUtils::to_string(position) + " V: " + ArrayUtils::to_string(velocity) + " mass: " + std::to_string(mass));
                list.emplace_back(position, velocity, mass);
            }
        }
    }
}

int CuboidGenerator::getX() const { return x; }

int CuboidGenerator::getY() const { return y; }

int CuboidGenerator::getZ() const { return z; }

double CuboidGenerator::getH() const { return h; }

double CuboidGenerator::getMass() const { return mass; }

double CuboidGenerator::getMeanBrownianMotion() const {
    return meanBrownianMotion;
}

const std::array<double, 3> &CuboidGenerator::getLLFC() const {
    return lowerLeftFrontCorner;
}

const std::array<double, 3> &CuboidGenerator::getInitialVelocity() const {
    return initialVelocity;
}
