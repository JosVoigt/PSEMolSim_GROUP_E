#include <gtest/gtest.h>

#include "container/Particle.h"
#include "force/LennardJonesMolecule.h"

#include <array>

#ifndef TOLERANCE
#define TOLERANCE 1e-7;
#endif

class LJM_test : public testing::Test {
   protected:
    std::array<double, 3> v = {0, 0, 0};
    std::array<double, 3> x1 = {1, 0, 0};
    std::array<double, 3> x2 = {0, 0, 0};

    Particle p1 = Particle(x1, v, 1, 0);
    Particle p2 = Particle(x2, v, 1, 0);

    double tolerance = TOLERANCE;
};

TEST_F(LJM_test, basic_functionality) {
    double const eps = 1;
    double const sigma = 1;

    LennardJonesMolecule ljm = LennardJonesMolecule(eps, sigma);

    std::array<double, 3> res = ljm.calculateForce(p1, p2);

    EXPECT_NEAR(24, res[0], tolerance);
    EXPECT_EQ(0.0, res[1]);
    EXPECT_EQ(0.0, res[2]);
}

TEST_F(LJM_test, sigma) {
    double const eps = 1;
    double const sigma = .5;

    LennardJonesMolecule ljm = LennardJonesMolecule(eps, sigma);

    std::array<double, 3> res = ljm.calculateForce(p1, p2);

    EXPECT_NEAR(-0.36328125, res[0], tolerance);
    EXPECT_EQ(0, res[1]);
    EXPECT_EQ(0, res[2]);
}

TEST_F(LJM_test, eps) {
    double const eps = .5;
    double const sigma = 1;

    LennardJonesMolecule ljm = LennardJonesMolecule(eps, sigma);

    std::array<double, 3> res = ljm.calculateForce(p1, p2);

    EXPECT_NEAR(12, res[0], tolerance);
    EXPECT_EQ(0, 0);
    EXPECT_EQ(0, 0);
}

TEST_F(LJM_test, all) {
    double const eps = 12;
    double const sigma = 7;

    std::array<double, 3> x1 = {4.7, 3.6, 2.43};
    Particle p1(x1, v, 1, 1);

    std::array<double, 3> x2 = {.895, 96.345, 23.4};
    Particle p2(x2, v, 1, 1);

    LennardJonesMolecule ljm(eps, sigma);

    std::array<double, 3> res = ljm.calculateForce(p1, p2);

    EXPECT_NEAR(-1.91698E-08, res[0], tolerance);
    EXPECT_NEAR(4.67254E-07, res[1], tolerance);
    EXPECT_NEAR(1.05648E-07, res[2], tolerance);
}
