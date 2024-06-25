#include <array>
#include <memory>

#include "io/logger/Logger.h"
#include "io/output/FileOutputHandler.h"
#include "particles/containers/ParticleContainer.h"
#include "particles/containers/directsum/DirectSumContainer.h"
#include "particles/containers/linkedcells/LinkedCellsContainer.h"
#include "particles/spawners/cuboid/CuboidSpawner.h"
#include "physics/pairwiseforces/GravitationalForce.h"
#include "physics/pairwiseforces/LennardJonesForce.h"
#include "simulation/Simulation.h"
#include "utils/ArrayUtils.h"

void execute2DRectBenchmark(int rect_width, int rect_height, double spacing, double lc_cutoff) {
    Logger::logger->set_level(spdlog::level::info);
    Logger::logger->info("Starting 2DRect-benchmark. Dimensions {}x{}...", rect_width, rect_height);

    // Settings for the Linked Cells Container simulation
    std::array<double, 3> domain_size = {300, 300, 3};
    std::array<LinkedCellsContainer::BoundaryCondition, 6> boundary_conditions = {
        LinkedCellsContainer::BoundaryCondition::REFLECTIVE, LinkedCellsContainer::BoundaryCondition::REFLECTIVE,
        LinkedCellsContainer::BoundaryCondition::REFLECTIVE, LinkedCellsContainer::BoundaryCondition::REFLECTIVE,
        LinkedCellsContainer::BoundaryCondition::REFLECTIVE, LinkedCellsContainer::BoundaryCondition::REFLECTIVE};

    // Settings for the Cuboid spawner for both simulations
    std::array<double, 3> center_offset = {domain_size[0] / 2, domain_size[1] / 2, domain_size[2] / 2};
    CuboidSpawner spawner(center_offset - std::array<double, 3>{rect_width * spacing / 2, rect_height * spacing / 2, 0},
                          {rect_width, rect_height, 1}, spacing, 1, {0, 0, 0}, 0);

    // Settings for the forces for both simulations
    std::vector<std::shared_ptr<PairwiseForceSource>> forces;
    forces.push_back(std::make_shared<LennardJonesForce>());

    // ############################################################
    // # Direct Sum Container
    // ############################################################

    std::vector<Particle> particles_ds;
    spawner.spawnParticles(particles_ds);

    // Instantiation of the Direct Sum Container simulation

    SimulationParams params_ds("2DParticleRect.xml", 0.01, 5, SimulationParams::DirectSumType{}, {}, {}, forces, true, true);

    params_ds.num_particles = particles_ds.size();

    Simulation simulation_ds(particles_ds, params_ds);

    // Simulating with Direct Sum Container
    params_ds.logSummary();
    SimulationOverview direct_sum_data = simulation_ds.runSimulation();
    direct_sum_data.logSummary();

    // ############################################################
    // # Linked Cells Container
    // ############################################################

    std::vector<Particle> particles_lc;
    spawner.spawnParticles(particles_lc);
    // Instantiation of the Linked Cells Container simulation
    SimulationParams params_lc{"2DParticleRect.xml",
                               0.01,
                               5,
                               SimulationParams::LinkedCellsType{domain_size, lc_cutoff, boundary_conditions},
                               {},
                               {},
                               forces,
                               true,
                               true};
    params_lc.num_particles = particles_lc.size();
    Simulation simulation_lc(particles_lc, params_lc);
    // Simulating with Linked Cells Container
    params_lc.logSummary();
    SimulationOverview linked_cells_data = simulation_lc.runSimulation();
    linked_cells_data.logSummary();

    // ############################################################
    // # Comparison Logging
    // ############################################################

    std::string ds_summary = "";
    for (auto& summary : direct_sum_data.interceptor_summaries) {
        ds_summary += summary + ";";
    }

    std::string lc_summary = "";
    for (auto& summary : linked_cells_data.interceptor_summaries) {
        lc_summary += summary + ";";
    }

    Logger::logger->info("Simulation of {} particles in a {}x{} grid\n", rect_width * rect_height, rect_width, rect_height);

    Logger::logger->info("Direct sum container:");
    Logger::logger->info("  Simulation took {:.3f}s", direct_sum_data.total_time_seconds);
    Logger::logger->info("  Total iterations: {}", direct_sum_data.total_iterations);
    Logger::logger->info("  Average time per iteration: {:.3f}ms\n", direct_sum_data.total_time_seconds / direct_sum_data.total_iterations);
    Logger::logger->info("  Summary {}", ds_summary);

    Logger::logger->info("Linked cells container:");
    Logger::logger->info("  Domain size: {:.0f}x{:.0f}x{:.0f}", domain_size[0], domain_size[1], domain_size[2]);
    Logger::logger->info("  Linked cells cutoff radius: {:.0f}", lc_cutoff);
    Logger::logger->info("  Simulation took {:.3f}s", linked_cells_data.total_time_seconds);
    Logger::logger->info("  Total iterations: {}", linked_cells_data.total_iterations);
    Logger::logger->info("  Average time per iteration: {:.3f}ms\n",
                         linked_cells_data.total_time_seconds / linked_cells_data.total_iterations);
    Logger::logger->info(" Summary {}", lc_summary);
}

/*
 * Creates a 2D particle rect with variable amount of particles and runs the simulation for time measurements.
 * Can be used to compare the performance of the different particle containers.
 */
int main() {
    std::vector<std::pair<int, int>> sizes = {{25, 40}, {50, 40}, {50, 80}, {100, 80}};

    for (auto [size_x, size_y] : sizes) {
        execute2DRectBenchmark(size_x, size_y, 1.1225, 3);
    }

    return 0;
}