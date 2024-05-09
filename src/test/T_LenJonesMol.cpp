#include <gtest/gtest.h>

#include "container/Particle.h"
#include "force/LennardJonesMolecule.h"

const double tolerance = 1e-7;

class LJM_test : public testing::Test {
   protected:
    std::array<double, 3> v = {0, 0, 0};
    std::array<double, 3> x1 = {1, 0, 0};
    std::array<double, 3> x2 = {0, 0, 0};

    Particle p1 = Particle(x1, v, 1, 0);
    Particle p2 = Particle(x2, v, 1, 0);
};

/*
 *                        /  /     sigma     \ 12     /     sigma     \ 6 \
 * U (x_i,x_j) = 4 * eps |  | -------------  |    -  | -------------  |   |
 *                       \  \ ||x_i - x_j|| /        \ ||x_i - x_j|| /   /
 */

TEST_F(LJM_test, basic_functionality) {
    double const eps = 1;
    double const sigma = 1;

    LennardJonesMolecule ljm = LennardJonesMolecule(eps, sigma);

    std::array<double, 3> res = ljm.calculateForce(p1, p2);

    ASSERT_NEAR(4, res[0], tolerance);
    ASSERT_EQ(0, res[1]);
    ASSERT_EQ(0, res[2]);
}

TEST_F(LJM_test, sigma) {
    double const eps = 1;
    double const sigma = .5;

    LennardJonesMolecule ljm = LennardJonesMolecule(eps, sigma);

    std::array<double, 3> res = ljm.calculateForce(p1, p2);

    ASSERT_NEAR(-0.0615234375, res[0], tolerance);
    ASSERT_EQ(0, res[1]);
    ASSERT_EQ(0, res[2]);
}

TEST_F(LJM_test, eps) {
    double const eps = .5;
    double const sigma = 2;

    LennardJonesMolecule ljm = LennardJonesMolecule(eps, sigma);

    std::array<double, 3> res = ljm.calculateForce(p1, p2);

    ASSERT_NEAR(8064, res[0], tolerance);
    ASSERT_EQ(0, res[1]);
    ASSERT_EQ(0, res[2]);
}