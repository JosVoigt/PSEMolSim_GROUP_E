#include <gtest/gtest.h>

#include "container/Particle.h"
#include "force/PlanetForce.h"

#ifndef TOLERANCE
#define TOLERANCE 1e-7;
#endif

class Planet_test : public testing::Test {
 protected:
  std::array<double, 3> v = {0, 0, 0};
  std::array<double, 3> x1_basic = {1, 0, 0};
  std::array<double, 3> x2_basic = {0, 0, 0};

  std::array<double, 3> x1_displaced = {5, 0, 0};
  std::array<double, 3> x2_displaced = {8, 0, 0};

  Particle planet1 = Particle(x1_basic, v, 1, 0);
  Particle planet2 = Particle(x2_basic, v, 1, 0);

  Particle planet1_displaced = Particle(x1_displaced, v, 1, 0);
  Particle planet2_displaced = Particle(x2_displaced, v, 1, 0);

  Particle planet1_displaced_and_mass = Particle(x1_displaced, v, 3.2, 0);
  Particle planet2_displaced_and_mass = Particle(x2_displaced, v, 6, 0);

  PlanetForce planet = PlanetForce();

  double tolerance = TOLERANCE;
};

TEST_F(Planet_test, basic_functionality) {
  std::array<double, 3> res = planet.calculateForce(planet1, planet2);

  EXPECT_EQ(-1, res[0]);
  EXPECT_EQ(0, res[1]);
  EXPECT_EQ(0, res[2]);
}

TEST_F(Planet_test, displacement) {
  std::array<double, 3> res =
      planet.calculateForce(planet1_displaced, planet2_displaced);

  EXPECT_NEAR(0.1111111, res[0], tolerance);
  EXPECT_EQ(0, res[1]);
  EXPECT_EQ(0, res[2]);
}

TEST_F(Planet_test, displacement_and_mass) {
  std::array<double, 3> res = planet.calculateForce(planet1_displaced_and_mass,
                                                    planet2_displaced_and_mass);

  EXPECT_NEAR(2.1333333, res[0], tolerance);
  EXPECT_EQ(0, res[1]);
  EXPECT_EQ(0, res[2]);
}
