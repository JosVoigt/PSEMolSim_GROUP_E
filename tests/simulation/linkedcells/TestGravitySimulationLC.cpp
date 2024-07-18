#include <gtest/gtest.h>

#include <memory>

#include "particles/containers/linkedcells/LinkedCellsContainer.h"
#include "simulation/Simulation.h"
#include "simulation/SimulationUtils.h"
#include "utils/ArrayUtils.h"

using BC = LinkedCellsContainer::BoundaryCondition;

/*
 * Test if the particles of a gravity simulation attract each other.
 */
TEST(SimulationRunnerLinkedCells, ParticlesAttractEachother_Gravity) {
    std::vector<Particle> particles;

    std::array<double, 3> center_offset = {5, 5, 5};

    std::array<double, 3> x1 = {0, 0, 0};
    std::array<double, 3> v1 = {0, 0, 0};

    std::array<double, 3> x2 = {1, 1, 1};
    std::array<double, 3> v2 = {0, 0, 0};

    auto initial_distance = ArrayUtils::L2Norm(x1 - x2);

    double mass = 1;

    auto p1 = Particle(x1 + center_offset, v1, mass, 0);
    auto p2 = Particle(x2 + center_offset, v2, mass, 0);

    particles.push_back(p1);
    particles.push_back(p2);

    SimulationParams params = TEST_DEFAULT_PARAMS_GRAVITY;
    params.end_time = 0.1;
    params.delta_t = 0.01;

    params.container_type =
        SimulationParams::LinkedCellsType({10, 10, 10}, 10, {BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW, BC::OUTFLOW});

    Simulation simulation(particles, params);

    auto res = simulation.runSimulation();

    auto new_dist = ArrayUtils::L2Norm((res.resulting_particles)[0].getX() - (res.resulting_particles)[1].getX());

    EXPECT_LT(new_dist, initial_distance);
}
