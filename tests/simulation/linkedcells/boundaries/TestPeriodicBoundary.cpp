#include <gtest/gtest.h>

#include <memory>

#include "particles/containers/ParticleContainer.h"
#include "particles/containers/linkedcells/LinkedCellsContainer.h"
#include "simulation/Simulation.h"
#include "simulation/SimulationUtils.h"

using BC = LinkedCellsContainer::BoundaryCondition;

/*
 * Test if the particle goes through the left boundary and comes out at the right.
 */
TEST(PeriodicBoundaryLC, GoThroughLeftBoundary) {
    std::vector<Particle> particles;

    std::array<double, 3> x1 = {2, 5, 5};
    std::array<double, 3> v1 = {-4, 0, 0};

    auto particle = Particle(x1, v1, 1, 0);

    particles.push_back(particle);

    SimulationParams params = TEST_DEFAULT_PARAMS_LENNARD_JONES;
    params.end_time = 1;
    params.delta_t = 0.005;

    params.container_type =
        SimulationParams::LinkedCellsType({10, 10, 10}, 5, {BC::PERIODIC, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW});

    Simulation simulation(particles, params);

    auto res = simulation.runSimulation();

    EXPECT_GE((res.resulting_particles)[0].getX()[0], 0);
    EXPECT_NEAR((res.resulting_particles)[0].getX()[0], 8, 1e-3);
}

/*
 * Test if the particle goes through the right boundary and comes out at the left.
 */
TEST(PeriodicBoundaryLC, GoThroughRightBoundary) {
    std::vector<Particle> particles;

    std::array<double, 3> x1 = {8, 5, 5};
    std::array<double, 3> v1 = {4, 0, 0};

    auto particle = Particle(x1, v1, 1, 0);

    particles.push_back(particle);

    SimulationParams params = TEST_DEFAULT_PARAMS_LENNARD_JONES;
    params.end_time = 1;
    params.delta_t = 0.005;

    params.container_type =
        SimulationParams::LinkedCellsType({10, 10, 10}, 5, {BC::OUTFLOW, BC::PERIODIC, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW});

    Simulation simulation(particles, params);

    auto res = simulation.runSimulation();

    EXPECT_LE((res.resulting_particles)[0].getX()[0], 8);
    EXPECT_NEAR((res.resulting_particles)[0].getX()[0], 2, 1e-3);
}

/*
 * Test if the particle goes through the back boundary and comes out at the front.
 */
TEST(PeriodicBoundaryLC, GoThroughBottomBoundary) {
    std::vector<Particle> particles;

    std::array<double, 3> x1 = {5, 2, 5};
    std::array<double, 3> v1 = {0, -4, 0};

    auto particle = Particle(x1, v1, 1, 0);

    particles.push_back(particle);

    SimulationParams params = TEST_DEFAULT_PARAMS_LENNARD_JONES;
    params.end_time = 1;
    params.delta_t = 0.005;

    params.container_type =
        SimulationParams::LinkedCellsType({10, 10, 10}, 5, {BC::OUTFLOW, BC::OUTFLOW, BC::PERIODIC, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW});

    Simulation simulation(particles, params);

    auto res = simulation.runSimulation();

    EXPECT_GE((res.resulting_particles)[0].getX()[1], 2);
    EXPECT_NEAR((res.resulting_particles)[0].getX()[1], 8, 1e-3);
}

/*
 * Test if the particle goes through the top boundary and comes out at the bottom.
 */
TEST(PeriodicBoundaryLC, GoThroughTopBoundary) {
    std::vector<Particle> particles;

    std::array<double, 3> x1 = {5, 8, 5};
    std::array<double, 3> v1 = {0, 4, 0};

    auto particle = Particle(x1, v1, 1, 0);

    particles.push_back(particle);

    SimulationParams params = TEST_DEFAULT_PARAMS_LENNARD_JONES;
    params.end_time = 1;
    params.delta_t = 0.005;

    params.container_type =
        SimulationParams::LinkedCellsType({10, 10, 10}, 5, {BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::PERIODIC, BC::OUTFLOW, BC::OUTFLOW});

    Simulation simulation(particles, params);

    auto res = simulation.runSimulation();

    EXPECT_LE((res.resulting_particles)[0].getX()[1], 8);
    EXPECT_NEAR((res.resulting_particles)[0].getX()[1], 2, 1e-3);
}

/*
 * Test if the particle goes through the back boundary and comes out at the front.
 */
TEST(PeriodicBoundaryLC, GoThroughBackBoundary) {
    std::vector<Particle> particles;

    std::array<double, 3> x1 = {5, 5, 2};
    std::array<double, 3> v1 = {0, 0, -4};

    auto particle = Particle(x1, v1, 1, 0);

    particles.push_back(particle);

    SimulationParams params = TEST_DEFAULT_PARAMS_LENNARD_JONES;
    params.end_time = 1;
    params.delta_t = 0.005;

    params.container_type =
        SimulationParams::LinkedCellsType({10, 10, 10}, 5, {BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::PERIODIC, BC::OUTFLOW});

    Simulation simulation(particles, params);

    auto res = simulation.runSimulation();

    EXPECT_GE((res.resulting_particles)[0].getX()[2], 2);
    EXPECT_NEAR((res.resulting_particles)[0].getX()[2], 8, 1e-3);
}

/*
 * Test if the particle goes through the front boundary and comes out at the back.
 */
TEST(PeriodicBoundaryLC, GoThroughFrontBoundary) {
    std::vector<Particle> particles;

    std::array<double, 3> x1 = {5, 5, 8};
    std::array<double, 3> v1 = {0, 0, 4};

    auto particle = Particle(x1, v1, 1, 0);

    particles.push_back(particle);

    SimulationParams params = TEST_DEFAULT_PARAMS_LENNARD_JONES;
    params.end_time = 1;
    params.delta_t = 0.005;

    params.container_type =
        SimulationParams::LinkedCellsType({10, 10, 10}, 5, {BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::PERIODIC});

    Simulation simulation(particles, params);

    auto res = simulation.runSimulation();

    EXPECT_LE((res.resulting_particles)[0].getX()[2], 8);
    EXPECT_NEAR((res.resulting_particles)[0].getX()[2], 2, 1e-3);
}

/*
 * Test if two particles affect each other at the boundary. (close together and pushed apart)
 */
TEST(PeriodicBoundaryLC, PushForceOverBoundarySides) {
    std::vector<Particle> particles;

    std::array<double, 3> x1 = {0.4, 5, 5};
    std::array<double, 3> x2 = {9.6, 5, 5};

    std::array<double, 3> y1 = {5, 0.4, 5};
    std::array<double, 3> y2 = {5, 9.6, 5};

    std::array<double, 3> z1 = {5, 5, 0.4};
    std::array<double, 3> z2 = {5, 5, 9.6};

    auto particle_x1 = Particle(x1, {0, 0, 0}, 1, 0);
    auto particle_x2 = Particle(x2, {0, 0, 0}, 1, 0);

    auto particle_y1 = Particle(y1, {0, 0, 0}, 1, 0);
    auto particle_y2 = Particle(y2, {0, 0, 0}, 1, 0);

    auto particle_z1 = Particle(z1, {0, 0, 0}, 1, 0);
    auto particle_z2 = Particle(z2, {0, 0, 0}, 1, 0);

    particles.push_back(particle_x1);
    particles.push_back(particle_x2);

    particles.push_back(particle_y1);
    particles.push_back(particle_y2);

    particles.push_back(particle_z1);
    particles.push_back(particle_z2);

    SimulationParams params = TEST_DEFAULT_PARAMS_LENNARD_JONES;
    params.end_time = 0.05;
    params.delta_t = 0.0005;

    params.container_type = SimulationParams::LinkedCellsType(
        {10, 10, 10}, 5, {BC::PERIODIC, BC::PERIODIC, BC::PERIODIC, BC::PERIODIC, BC::PERIODIC, BC::PERIODIC});

    Simulation simulation(particles, params);

    auto res = simulation.runSimulation();

    EXPECT_GT(res.resulting_particles[0].getX()[0], 0.4);
    EXPECT_GT(res.resulting_particles[0].getV()[0], 0);
    EXPECT_LT(res.resulting_particles[1].getX()[0], 9.6);
    EXPECT_LT(res.resulting_particles[1].getV()[0], 0);

    EXPECT_GT(res.resulting_particles[2].getX()[1], 0.4);
    EXPECT_GT(res.resulting_particles[2].getV()[1], 0);
    EXPECT_LT(res.resulting_particles[3].getX()[1], 9.6);
    EXPECT_LT(res.resulting_particles[3].getV()[1], 0);

    EXPECT_GT(res.resulting_particles[4].getX()[2], 0.4);
    EXPECT_GT(res.resulting_particles[4].getV()[2], 0);
    EXPECT_LT(res.resulting_particles[5].getX()[2], 9.6);
    EXPECT_LT(res.resulting_particles[5].getV()[2], 0);
}

/*
 * Test if particles affect each other at the boundary corners. (close together and pushed apart)
 */
TEST(PeriodicBoundaryLC, PushForceOverAllBoundaryCorners) {
    std::vector<Particle> particles;

    std::array<double, 3> x1 = {0.3, 0.3, 0.3};
    std::array<double, 3> x2 = {0.3, 0.3, 9.7};
    std::array<double, 3> x3 = {0.3, 9.7, 0.3};
    std::array<double, 3> x4 = {0.3, 9.7, 9.7};
    std::array<double, 3> x5 = {9.7, 0.3, 0.3};
    std::array<double, 3> x6 = {9.7, 0.3, 9.7};
    std::array<double, 3> x7 = {9.7, 9.7, 0.3};
    std::array<double, 3> x8 = {9.7, 9.7, 9.7};

    auto particle_x1 = Particle(x1, {0, 0, 0}, 1, 0);
    auto particle_x2 = Particle(x2, {0, 0, 0}, 1, 0);
    auto particle_x3 = Particle(x3, {0, 0, 0}, 1, 0);
    auto particle_x4 = Particle(x4, {0, 0, 0}, 1, 0);
    auto particle_x5 = Particle(x5, {0, 0, 0}, 1, 0);
    auto particle_x6 = Particle(x6, {0, 0, 0}, 1, 0);
    auto particle_x7 = Particle(x7, {0, 0, 0}, 1, 0);
    auto particle_x8 = Particle(x8, {0, 0, 0}, 1, 0);

    particles.push_back(particle_x1);
    particles.push_back(particle_x2);
    particles.push_back(particle_x3);
    particles.push_back(particle_x4);
    particles.push_back(particle_x5);
    particles.push_back(particle_x6);
    particles.push_back(particle_x7);
    particles.push_back(particle_x8);

    SimulationParams params = TEST_DEFAULT_PARAMS_LENNARD_JONES;
    params.end_time = 0.005;
    params.delta_t = 0.0005;

    params.container_type = SimulationParams::LinkedCellsType(
        {10, 10, 10}, 5, {BC::PERIODIC, BC::PERIODIC, BC::PERIODIC, BC::PERIODIC, BC::PERIODIC, BC::PERIODIC});

    Simulation simulation(particles, params);

    auto res = simulation.runSimulation();

    EXPECT_GT(res.resulting_particles[0].getX()[0], 0.3);
    EXPECT_GT(res.resulting_particles[0].getX()[1], 0.3);
    EXPECT_GT(res.resulting_particles[0].getX()[2], 0.3);

    EXPECT_GT(res.resulting_particles[1].getX()[0], 0.3);
    EXPECT_GT(res.resulting_particles[1].getX()[1], 0.3);
    EXPECT_LT(res.resulting_particles[1].getX()[2], 9.7);

    EXPECT_GT(res.resulting_particles[2].getX()[0], 0.3);
    EXPECT_LT(res.resulting_particles[2].getX()[1], 9.7);
    EXPECT_GT(res.resulting_particles[2].getX()[2], 0.3);

    EXPECT_GT(res.resulting_particles[3].getX()[0], 0.3);
    EXPECT_LT(res.resulting_particles[3].getX()[1], 9.7);
    EXPECT_LT(res.resulting_particles[3].getX()[2], 9.7);

    EXPECT_LT(res.resulting_particles[4].getX()[0], 9.7);
    EXPECT_GT(res.resulting_particles[4].getX()[1], 0.3);
    EXPECT_GT(res.resulting_particles[4].getX()[2], 0.3);

    EXPECT_LT(res.resulting_particles[5].getX()[0], 9.7);
    EXPECT_GT(res.resulting_particles[5].getX()[1], 0.3);
    EXPECT_LT(res.resulting_particles[5].getX()[2], 9.7);

    EXPECT_LT(res.resulting_particles[6].getX()[0], 9.7);
    EXPECT_LT(res.resulting_particles[6].getX()[1], 9.7);
    EXPECT_GT(res.resulting_particles[6].getX()[2], 0.3);

    EXPECT_LT(res.resulting_particles[7].getX()[0], 9.7);
    EXPECT_LT(res.resulting_particles[7].getX()[1], 9.7);
    EXPECT_LT(res.resulting_particles[7].getX()[2], 9.7);

    EXPECT_GT(res.resulting_particles[0].getV()[0], 0);
    EXPECT_GT(res.resulting_particles[0].getV()[1], 0);
    EXPECT_GT(res.resulting_particles[0].getV()[2], 0);

    EXPECT_GT(res.resulting_particles[1].getV()[0], 0);
    EXPECT_GT(res.resulting_particles[1].getV()[1], 0);
    EXPECT_LT(res.resulting_particles[1].getV()[2], 0);

    EXPECT_GT(res.resulting_particles[2].getV()[0], 0);
    EXPECT_LT(res.resulting_particles[2].getV()[1], 0);
    EXPECT_GT(res.resulting_particles[2].getV()[2], 0);

    EXPECT_GT(res.resulting_particles[3].getV()[0], 0);
    EXPECT_LT(res.resulting_particles[3].getV()[1], 0);
    EXPECT_LT(res.resulting_particles[3].getV()[2], 0);

    EXPECT_LT(res.resulting_particles[4].getV()[0], 0);
    EXPECT_GT(res.resulting_particles[4].getV()[1], 0);
    EXPECT_GT(res.resulting_particles[4].getV()[2], 0);

    EXPECT_LT(res.resulting_particles[5].getV()[0], 0);
    EXPECT_GT(res.resulting_particles[5].getV()[1], 0);
    EXPECT_LT(res.resulting_particles[5].getV()[2], 0);

    EXPECT_LT(res.resulting_particles[6].getV()[0], 0);
    EXPECT_LT(res.resulting_particles[6].getV()[1], 0);
    EXPECT_GT(res.resulting_particles[6].getV()[2], 0);

    EXPECT_LT(res.resulting_particles[7].getV()[0], 0);
    EXPECT_LT(res.resulting_particles[7].getV()[1], 0);
    EXPECT_LT(res.resulting_particles[7].getV()[2], 0);
}

/*
 * Test if particles affect each other at diagonal boundary edges. (close together and pushed apart)
 */
TEST(PeriodicBoundaryLC, PushForceOverDiagonalBoundaryEdge) {
    std::vector<Particle> particles;

    std::array<double, 3> x1 = {0.3, 0.3, 5};
    std::array<double, 3> x2 = {9.7, 9.7, 5};

    auto particle_x1 = Particle(x1, {0, 0, 0}, 1, 0);
    auto particle_x2 = Particle(x2, {0, 0, 0}, 1, 0);

    particles.push_back(particle_x1);
    particles.push_back(particle_x2);

    SimulationParams params = TEST_DEFAULT_PARAMS_LENNARD_JONES;
    params.end_time = 0.05;
    params.delta_t = 0.0005;

    params.container_type = SimulationParams::LinkedCellsType(
        {10, 10, 10}, 5, {BC::PERIODIC, BC::PERIODIC, BC::PERIODIC, BC::PERIODIC, BC::PERIODIC, BC::PERIODIC});

    Simulation simulation(particles, params);

    auto res = simulation.runSimulation();

    EXPECT_GT(res.resulting_particles[0].getX()[0], 0.3);
    EXPECT_GT(res.resulting_particles[0].getX()[1], 0.3);

    EXPECT_LT(res.resulting_particles[1].getX()[0], 9.7);
    EXPECT_LT(res.resulting_particles[1].getX()[1], 9.7);

    EXPECT_GT(res.resulting_particles[0].getV()[0], 0);
    EXPECT_GT(res.resulting_particles[0].getV()[1], 0);

    EXPECT_LT(res.resulting_particles[1].getV()[0], 0);
    EXPECT_LT(res.resulting_particles[1].getV()[1], 0);
}

/*
 * Test if particles affect each other at the boundary corners. (close together and pushed apart)
 */
TEST(PeriodicBoundaryLC, PushForceOverDiagonalBoundaryCorner) {
    std::vector<Particle> particles;

    std::array<double, 3> x1 = {0.3, 0.3, 0.3};
    std::array<double, 3> x2 = {9.7, 9.7, 9.7};

    auto particle_x1 = Particle(x1, {0, 0, 0}, 1, 0);
    auto particle_x2 = Particle(x2, {0, 0, 0}, 1, 0);

    particles.push_back(particle_x1);
    particles.push_back(particle_x2);

    SimulationParams params = TEST_DEFAULT_PARAMS_LENNARD_JONES;
    params.end_time = 0.05;
    params.delta_t = 0.0005;

    params.container_type = SimulationParams::LinkedCellsType(
        {10, 10, 10}, 5, {BC::PERIODIC, BC::PERIODIC, BC::PERIODIC, BC::PERIODIC, BC::PERIODIC, BC::PERIODIC});

    Simulation simulation(particles, params);

    auto res = simulation.runSimulation();

    EXPECT_GT(res.resulting_particles[0].getX()[0], 0.3);
    EXPECT_GT(res.resulting_particles[0].getX()[1], 0.3);
    EXPECT_GT(res.resulting_particles[0].getX()[2], 0.3);

    EXPECT_LT(res.resulting_particles[1].getX()[0], 9.7);
    EXPECT_LT(res.resulting_particles[1].getX()[1], 9.7);
    EXPECT_LT(res.resulting_particles[1].getX()[2], 9.7);

    EXPECT_GT(res.resulting_particles[0].getV()[0], 0);
    EXPECT_GT(res.resulting_particles[0].getV()[1], 0);
    EXPECT_GT(res.resulting_particles[0].getV()[2], 0);

    EXPECT_LT(res.resulting_particles[1].getV()[0], 0);
    EXPECT_LT(res.resulting_particles[1].getV()[1], 0);
    EXPECT_LT(res.resulting_particles[1].getV()[2], 0);
}

/*
 * Test if particles affect each other at the boundary corners. (close together and pushed apart)
 */
TEST(PeriodicBoundaryLC, PushForceOverDiagonalSideCorner) {
    std::vector<Particle> particles;

    std::array<double, 3> x1 = {0.3, 0.3, 0.3};
    std::array<double, 3> x2 = {9.7, 9.7, 0.3};

    auto particle_x1 = Particle(x1, {0, 0, 0}, 1, 0);
    auto particle_x2 = Particle(x2, {0, 0, 0}, 1, 0);

    particles.push_back(particle_x1);
    particles.push_back(particle_x2);

    SimulationParams params = TEST_DEFAULT_PARAMS_LENNARD_JONES;
    params.end_time = 0.05;
    params.delta_t = 0.0005;

    params.container_type = SimulationParams::LinkedCellsType(
        {10, 10, 10}, 5, {BC::PERIODIC, BC::PERIODIC, BC::PERIODIC, BC::PERIODIC, BC::PERIODIC, BC::PERIODIC});

    Simulation simulation(particles, params);

    auto res = simulation.runSimulation();

    EXPECT_GT(res.resulting_particles[0].getX()[0], 0.3);
    EXPECT_GT(res.resulting_particles[0].getX()[1], 0.3);
    EXPECT_NEAR(res.resulting_particles[0].getX()[2], 0.3, 1e-7);

    EXPECT_LT(res.resulting_particles[1].getX()[0], 9.7);
    EXPECT_LT(res.resulting_particles[1].getX()[1], 9.7);
    EXPECT_NEAR(res.resulting_particles[1].getX()[2], 0.3, 1e-7);

    EXPECT_GT(res.resulting_particles[0].getV()[0], 0);
    EXPECT_GT(res.resulting_particles[0].getV()[1], 0);
    EXPECT_NEAR(res.resulting_particles[0].getV()[2], 0, 1e-7);

    EXPECT_LT(res.resulting_particles[1].getV()[0], 0);
    EXPECT_LT(res.resulting_particles[1].getV()[1], 0);
    EXPECT_NEAR(res.resulting_particles[1].getV()[2], 0, 1e-7);
}