#include <gtest/gtest.h>

#include "io/output/FileOutputHandler.h"
#include "particles/containers/ParticleContainer.h"
#include "simulation/Simulation.h"
#include "simulation/SimulationUtils.h"

#define EXPECT_ARRAY_NEAR(a, b, tol)  \
    for (int i = 0; i < 3; i++) {     \
        EXPECT_NEAR(a[i], b[i], tol); \
    };

/*
 * Test if the particles return to their initial position after one period.
 * This test relies on the periodic solution presented in: http://three-body.ipb.ac.rs/sV_sol.php?id=0
 */
TEST(SimulationRunnerDirectSum, ParticlesReturnToInitialPositionPeriodicSolution_Gravity) {
    std::vector<Particle> particles;

    auto p1 = 0.347113;
    auto p2 = 0.532727;
    auto period = 6.325897;

    std::array<double, 3> x1 = {-1, 0, 0};
    std::array<double, 3> v1 = {p1, p2, 0};

    std::array<double, 3> x2 = {1, 0, 0};
    std::array<double, 3> v2 = {p1, p2, 0};

    std::array<double, 3> x3 = {0, 0, 0};
    std::array<double, 3> v3 = {-2 * p1, -2 * p2, 0};

    auto pa1 = Particle(x1, v1, 1, 0);
    auto pa2 = Particle(x2, v2, 1, 0);
    auto pa3 = Particle(x3, v3, 1, 0);

    particles.push_back(pa1);
    particles.push_back(pa2);
    particles.push_back(pa3);

    SimulationParams params = TEST_DEFAULT_PARAMS_GRAVITY;
    params.end_time = period;
    params.delta_t = 0.001;

    Simulation simulation(particles, params);

    auto result = simulation.runSimulation();

    EXPECT_ARRAY_NEAR((result.resulting_particles)[0].getX(), x1, 0.01);
    EXPECT_ARRAY_NEAR((result.resulting_particles)[1].getX(), x2, 0.01);
    EXPECT_ARRAY_NEAR((result.resulting_particles)[2].getX(), x3, 0.01);
}
