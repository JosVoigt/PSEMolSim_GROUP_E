#include <gtest/gtest.h>

#include <memory>

#include "particles/containers/ParticleContainer.h"
#include "particles/containers/linkedcells/LinkedCellsContainer.h"
#include "simulation/Simulation.h"
#include "simulation/SimulationUtils.h"

using BC = LinkedCellsContainer::BoundaryCondition;

/*
 * Test if the particle is reflected at the left boundary.
 */
TEST(ReflectiveBoundaryLC, BouncesAtLeftBoundary) {
    std::vector<Particle> particles;

    std::array<double, 3> x1 = {2, 5, 5};
    std::array<double, 3> v1 = {-4, 0, 0};

    auto particle = Particle(x1, v1, 1, 0);

    particles.push_back(particle);

    SimulationParams params = TEST_DEFAULT_PARAMS_LENNARD_JONES;
    params.end_time = 0.5;
    params.delta_t = 0.005;

    params.container_type = SimulationParams::LinkedCellsType(
        {10, 10, 10}, 5, {BC::REFLECTIVE, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW});

    Simulation simulation(particles, params);

    auto res = simulation.runSimulation();

    EXPECT_GE((res.resulting_particles)[0].getV()[0], 0);
    EXPECT_NEAR((res.resulting_particles)[0].getV()[0], -v1[0], 1e-3);
}

/*
 * Test if the particle is reflected at the right boundary.
 */
TEST(ReflectiveBoundaryLC, BouncesAtRightBoundary) {
    std::vector<Particle> particles;

    std::array<double, 3> x1 = {8, 5, 5};
    std::array<double, 3> v1 = {4, 0, 0};

    auto particle = Particle(x1, v1, 1, 0);

    particles.push_back(particle);

    SimulationParams params = TEST_DEFAULT_PARAMS_LENNARD_JONES;
    params.end_time = 0.5;
    params.delta_t = 0.005;

    params.container_type = SimulationParams::LinkedCellsType(
        {10, 10, 10}, 5, {BC::OUTFLOW, BC::REFLECTIVE, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW});

    Simulation simulation(particles, params);

    auto res = simulation.runSimulation();

    EXPECT_LE((res.resulting_particles)[0].getV()[0], 0);
    EXPECT_NEAR((res.resulting_particles)[0].getV()[0], -v1[0], 1e-3);
}

/*
 * Test if the particle is reflected at the bottom boundary.
 */
TEST(ReflectiveBoundaryLC, BouncesAtBottomBoundary) {
    std::vector<Particle> particles;

    std::array<double, 3> x1 = {5, 2, 5};
    std::array<double, 3> v1 = {0, -4, 0};

    auto particle = Particle(x1, v1, 1, 0);

    particles.push_back(particle);

    SimulationParams params = TEST_DEFAULT_PARAMS_LENNARD_JONES;
    params.end_time = 0.5;
    params.delta_t = 0.005;

    params.container_type = SimulationParams::LinkedCellsType(
        {10, 10, 10}, 5, {BC::OUTFLOW, BC::OUTFLOW, BC::REFLECTIVE, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW});

    Simulation simulation(particles, params);

    auto res = simulation.runSimulation();

    EXPECT_GE((res.resulting_particles)[0].getV()[1], 0);
    EXPECT_NEAR((res.resulting_particles)[0].getV()[1], -v1[1], 1e-3);
}

/*
 * Test if the particle is reflected at the top boundary.
 */
TEST(ReflectiveBoundaryLC, BouncesAtTopBoundary) {
    std::vector<Particle> particles;

    std::array<double, 3> x1 = {5, 8, 5};
    std::array<double, 3> v1 = {0, 4, 0};

    auto particle = Particle(x1, v1, 1, 0);

    particles.push_back(particle);

    SimulationParams params = TEST_DEFAULT_PARAMS_LENNARD_JONES;
    params.end_time = 0.5;
    params.delta_t = 0.005;

    params.container_type = SimulationParams::LinkedCellsType(
        {10, 10, 10}, 5, {BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::REFLECTIVE, BC::OUTFLOW, BC::OUTFLOW});

    Simulation simulation(particles, params);

    auto res = simulation.runSimulation();

    EXPECT_LE((res.resulting_particles)[0].getV()[1], 0);
    EXPECT_NEAR((res.resulting_particles)[0].getV()[1], -v1[1], 1e-3);
}

/*
 * Test if the particle is reflected at the back boundary.
 */
TEST(ReflectiveBoundaryLC, BouncesAtBackBoundary) {
    std::vector<Particle> particles;

    std::array<double, 3> x1 = {5, 5, 2};
    std::array<double, 3> v1 = {0, 0, -4};

    auto particle = Particle(x1, v1, 1, 0);

    particles.push_back(particle);

    SimulationParams params = TEST_DEFAULT_PARAMS_LENNARD_JONES;
    params.end_time = 0.5;
    params.delta_t = 0.005;

    params.container_type = SimulationParams::LinkedCellsType(
        {10, 10, 10}, 5, {BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::REFLECTIVE, BC::OUTFLOW});

    Simulation simulation(particles, params);

    auto res = simulation.runSimulation();

    EXPECT_GE((res.resulting_particles)[0].getV()[2], 0);
    EXPECT_NEAR((res.resulting_particles)[0].getV()[2], -v1[2], 1e-3);
}

/*
 * Test if the particle is reflected at the front boundary.
 */
TEST(ReflectiveBoundaryLC, BouncesAtFrontBoundary) {
    std::vector<Particle> particles;

    std::array<double, 3> x1 = {5, 5, 8};
    std::array<double, 3> v1 = {0, 0, 4};

    auto particle = Particle(x1, v1, 1, 0);

    particles.push_back(particle);

    SimulationParams params = TEST_DEFAULT_PARAMS_LENNARD_JONES;
    params.end_time = 0.5;
    params.delta_t = 0.005;

    params.container_type = SimulationParams::LinkedCellsType(
        {10, 10, 10}, 5, {BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::REFLECTIVE});

    Simulation simulation(particles, params);

    auto res = simulation.runSimulation();

    EXPECT_LE((res.resulting_particles)[0].getV()[2], 0);
    EXPECT_NEAR((res.resulting_particles)[0].getV()[2], -v1[2], 1e-3);
}
