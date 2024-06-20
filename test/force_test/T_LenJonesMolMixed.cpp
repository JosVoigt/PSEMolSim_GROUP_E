#include <gtest/gtest.h>

#include <array>
#include <cmath>

#include "container/Particle.h"
#include "force/LenJonesForceMixed.h"
#include "force/LennardJonesForce.h"

class LJM_Mixed_Test : public testing::Test {
 protected:
  LennardJonesForceMixed ljfm =
      LennardJonesForceMixed({1, 2, 3}, {1, 2, 3}, {1, 2, 3});
};

TEST_F(LJM_Mixed_Test, SameParticle) {
  std::array<double, 3> zero = {0, 0, 0};
  std::array<double, 3> one = {1, 0, 0};

  Particle p1 = Particle(zero, zero, 0, 1);
  Particle p2 = Particle(one, zero, 0, 1);

  std::array<double, 3> force = ljfm.calculateForce(p1, p2);

  LennardJonesForce ljm = LennardJonesForce(1, 1);
  std::array<double, 3> ljm_force = ljm.calculateForce(p1, p2);

  EXPECT_NEAR(force[0], ljm_force[0], 10e-7);
  EXPECT_NEAR(force[1], ljm_force[1], 10e-7);
  EXPECT_NEAR(force[2], ljm_force[2], 10e-7);
}

TEST_F(LJM_Mixed_Test, DifferentParticles) {
  std::array<double, 3> zero = {0, 0, 0};
  std::array<double, 3> one = {1, 0, 0};

  Particle p1 = Particle(zero, zero, 0, 1);
  Particle p2 = Particle(one, zero, 0, 3);

  std::array<double, 3> force = ljfm.calculateForce(p1, p2);

  LennardJonesForce ljm = LennardJonesForce(std::sqrt(3), 2);
  std::array<double, 3> ljm_force = ljm.calculateForce(p1, p2);

  EXPECT_NEAR(force[0], ljm_force[0], 10e-7);
  EXPECT_NEAR(force[1], ljm_force[1], 10e-7);
  EXPECT_NEAR(force[2], ljm_force[2], 10e-7);
}
