#include <gtest/gtest.h>

#include <memory>
#include <random>

#include "particles/Particle.h"
#include "particles/containers/directsum/DirectSumContainer.h"
#include "particles/containers/linkedcells/LinkedCellsContainer.h"
#include "physics/thermostats/Thermostat.h"
#include "utils/ArrayUtils.h"

/**
 * Create a standard LinkedCellsContainer with dimensions 1000x1000x1000, cutoff radius 100 and reflective boundary conditions.
 */
std::unique_ptr<ParticleContainer> createLinkedCellsContainer() {
    std::array<double, 3> domain_size = {1000, 1000, 1000};
    double cutoff_radius = 100;
    std::array<LinkedCellsContainer::BoundaryCondition, 6> boundary_conditions = {
        LinkedCellsContainer::BoundaryCondition::REFLECTIVE, LinkedCellsContainer::BoundaryCondition::REFLECTIVE,
        LinkedCellsContainer::BoundaryCondition::REFLECTIVE, LinkedCellsContainer::BoundaryCondition::REFLECTIVE,
        LinkedCellsContainer::BoundaryCondition::REFLECTIVE, LinkedCellsContainer::BoundaryCondition::REFLECTIVE};

    return std::make_unique<LinkedCellsContainer>(domain_size, cutoff_radius, boundary_conditions);
}

/**
 * Generate particles in a grid with the given dimensions, 0 velocity and random mass between 1 and 5 and add them to the particle
 * container.
 */
void generateParticles(size_t x, size_t y, size_t z, std::unique_ptr<ParticleContainer>& particle_container) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(1, 5);

    for (size_t cx = 1; cx <= x; ++cx) {
        for (size_t cy = 1; cy <= y; ++cy) {
            for (size_t cz = 1; cz <= z; ++cz) {
                std::array<double, 3> x = {2.0 * cx, 2.0 * cy, 2.0 * cz};
                double m = dist(gen);
                particle_container->addParticle(Particle(x, {0, 0, 0}, m, 0));
            }
        }
    }
}

/*
 * Test kinetic energy calculation manually
 */
TEST(Thermostat, KineticEnergyCalculation) {
    std::array<double, 3> x1 = {0, 0, 0};
    std::array<double, 3> v1 = {1, 0, 2};
    double m1 = 1;

    std::array<double, 3> x2 = {1, 1, 1};
    std::array<double, 3> v2 = {-0.5, 0.5, 0.5};
    double m2 = 4;

    // Kinetic energy = 0.5 * m * <v, v>

    // Particle 1: E_kin = 0.5 * 1 * (1 + 0 + 4) = 2.5
    auto p1 = Particle(x1, v1, m1, 0);

    // Particle 2: E_kin = 0.5 * 4 * (0.25 + 0.25 + 0.25) = 1.5
    auto p2 = Particle(x2, v2, m2, 1);

    // E_kin_total = 2.5 + 1.5 = 4

    std::unique_ptr<ParticleContainer> particle_container = std::make_unique<DirectSumContainer>();
    particle_container->addParticle(p1);
    particle_container->addParticle(p2);

    auto thermostat = Thermostat(1, 1, 1, true);

    EXPECT_NEAR(thermostat.getKineticEnergy(particle_container), 4, 1e-6);
}

/*
 * Test temperature calculation manually
 */
TEST(Thermostat, TemperatureCalculation3D) {
    std::array<double, 3> x1 = {0, 0, 0};
    std::array<double, 3> v1 = {1, 0, 2};
    double m1 = 1;

    std::array<double, 3> x2 = {1, 1, 1};
    std::array<double, 3> v2 = {-0.5, 0.5, 0.5};
    double m2 = 4;

    // Temperature = (2 * E_kin) / (#dimensions * #particles * k_B)  | k_B = 1 for us

    // Particle 1: E_kin = 0.5 * 1 * (1 + 0 + 4) = 2.5
    auto p1 = Particle(x1, v1, m1, 0);

    // Particle 2: E_kin = 0.5 * 4 * (0.25 + 0.25 + 0.25) = 1.5
    auto p2 = Particle(x2, v2, m2, 1);

    // E_kin_total = 4 | #dimensions = 3 | #particles = 2 | k_B = 1
    // Temperature = (2 * 4) / (3 * 2 * 1) = 7.5 / 6 = 1.33333

    std::unique_ptr<ParticleContainer> particle_container = std::make_unique<DirectSumContainer>();
    particle_container->addParticle(p1);
    particle_container->addParticle(p2);

    auto thermostat = Thermostat(1, 1, 1, true);

    EXPECT_NEAR(thermostat.getCurrentTemperature(particle_container), 4.0 / 3, 1e-6);
}

/*
 * Test temperature calculation manually
 */
TEST(Thermostat, TemperatureCalculation2D) {
    std::array<double, 3> x1 = {0, 0, 0};
    std::array<double, 3> v1 = {1, 2, 0};
    double m1 = 1;

    std::array<double, 3> x2 = {1, 1, 0};
    std::array<double, 3> v2 = {-0.5, 0.5, 0};
    double m2 = 4;

    // Temperature = (2 * E_kin) / (#dimensions * #particles * k_B)  | k_B = 1 for us

    // Particle 1: E_kin = 0.5 * 1 * (1 + 4) = 2.5
    auto p1 = Particle(x1, v1, m1, 0);

    // Particle 2: E_kin = 0.5 * 4 * (0.25 + 0.25) = 1
    auto p2 = Particle(x2, v2, m2, 1);

    // E_kin_total = 3.5 | #dimensions = 2 | #particles = 2 | k_B = 1
    // Temperature = (2 * 3.5) / (2 * 2 * 1) = 7 / 4 = 1.75

    std::unique_ptr<ParticleContainer> particle_container = std::make_unique<DirectSumContainer>();
    particle_container->addParticle(p1);
    particle_container->addParticle(p2);

    auto thermostat = Thermostat(1, 1, 1, false);

    EXPECT_NEAR(thermostat.getCurrentTemperature(particle_container), 1.75, 1e-6);
}

/**
 * Test temperature setting
 */
TEST(Thermostat, SetTemperature) {
    auto thermostat = Thermostat(1, 1, 1, true);

    // Test with DirectSumContainer

    std::unique_ptr<ParticleContainer> particle_container_ds = std::make_unique<DirectSumContainer>();
    generateParticles(50, 50, 50, particle_container_ds);

    // Initial temperature should be 0 since no particle has a velocity
    EXPECT_NEAR(thermostat.getCurrentTemperature(particle_container_ds), 0, 1e-6);
    thermostat.setTemperature(2, particle_container_ds);

    // Temperature should be around 2 after setting the temperature
    EXPECT_NEAR(thermostat.getCurrentTemperature(particle_container_ds), 2, 1e-2);

    // Test with LinkedCellsContainer

    std::unique_ptr<ParticleContainer> particle_container_lc = createLinkedCellsContainer();
    generateParticles(50, 50, 50, particle_container_lc);

    // Initial temperature should be 0 since no particle has a velocity
    EXPECT_NEAR(thermostat.getCurrentTemperature(particle_container_lc), 0, 1e-6);
    thermostat.setTemperature(2, particle_container_lc);

    // Temperature should be around 2 after setting the temperature
    EXPECT_NEAR(thermostat.getCurrentTemperature(particle_container_lc), 2, 1e-2);
}

/*
 * Test temperature scaling heating
 */
TEST(Thermostat, TemperatureHeatingFullStep) {
    auto thermostat = Thermostat(5, 5, 1, true);

    // Test with DirectSumContainer

    std::unique_ptr<ParticleContainer> particle_container_ds = std::make_unique<DirectSumContainer>();
    generateParticles(50, 50, 50, particle_container_ds);
    thermostat.setTemperature(1, particle_container_ds);

    // Initial temperature should be 1
    EXPECT_NEAR(thermostat.getCurrentTemperature(particle_container_ds), 1, 1e-2);

    // Scale temperature to 5
    thermostat.scaleTemperature(particle_container_ds);

    // Temperature should be around 5 after scaling
    EXPECT_NEAR(thermostat.getCurrentTemperature(particle_container_ds), 5, 1e-2);

    // Test with LinkedCellsContainer

    std::unique_ptr<ParticleContainer> particle_container_lc = createLinkedCellsContainer();
    generateParticles(50, 50, 50, particle_container_lc);
    thermostat.setTemperature(1, particle_container_lc);

    // Initial temperature should be 1
    EXPECT_NEAR(thermostat.getCurrentTemperature(particle_container_lc), 1, 1e-2);

    // Scale temperature to 5
    thermostat.scaleTemperature(particle_container_lc);

    // Temperature should be around 5 after scaling
    EXPECT_NEAR(thermostat.getCurrentTemperature(particle_container_lc), 5, 1e-2);
}

/*
 * Test temperature scaling heating
 */
TEST(Thermostat, TemperatureHeatingCappedStep) {
    auto thermostat = Thermostat(5, 0.5, 1, true);

    // Test with DirectSumContainer

    std::unique_ptr<ParticleContainer> particle_container_ds = std::make_unique<DirectSumContainer>();
    generateParticles(50, 50, 50, particle_container_ds);
    thermostat.setTemperature(1, particle_container_ds);

    // Initial temperature should be 1
    EXPECT_NEAR(thermostat.getCurrentTemperature(particle_container_ds), 1, 1e-2);

    // Scale temperature to 5
    thermostat.scaleTemperature(particle_container_ds);

    // Temperature should be around 1.5 after scaling since the maximum temperature change is 0.5
    EXPECT_NEAR(thermostat.getCurrentTemperature(particle_container_ds), 1.5, 1e-2);

    // Test with LinkedCellsContainer

    std::unique_ptr<ParticleContainer> particle_container_lc = createLinkedCellsContainer();
    generateParticles(50, 50, 50, particle_container_lc);
    thermostat.setTemperature(1, particle_container_lc);

    // Initial temperature should be 1
    EXPECT_NEAR(thermostat.getCurrentTemperature(particle_container_lc), 1, 1e-2);

    // Scale temperature to 5
    thermostat.scaleTemperature(particle_container_lc);

    // Temperature should be around 1.5 after scaling since the maximum temperature change is 0.5
    EXPECT_NEAR(thermostat.getCurrentTemperature(particle_container_lc), 1.5, 1e-2);
}

/*
 * Test temperature scaling heating
 */
TEST(Thermostat, TemperatureCoolingFullStep) {
    auto thermostat = Thermostat(5, 5, 1, true);

    // Test with DirectSumContainer

    std::unique_ptr<ParticleContainer> particle_container_ds = std::make_unique<DirectSumContainer>();
    generateParticles(50, 50, 50, particle_container_ds);
    thermostat.setTemperature(9, particle_container_ds);

    // Initial temperature should be 9
    EXPECT_NEAR(thermostat.getCurrentTemperature(particle_container_ds), 9, 3e-2);

    // Scale temperature to 5
    thermostat.scaleTemperature(particle_container_ds);

    // Temperature should be around 5 after scaling
    EXPECT_NEAR(thermostat.getCurrentTemperature(particle_container_ds), 5, 3e-2);

    // Test with LinkedCellsContainer

    std::unique_ptr<ParticleContainer> particle_container_lc = createLinkedCellsContainer();
    generateParticles(50, 50, 50, particle_container_lc);
    thermostat.setTemperature(9, particle_container_lc);

    // Initial temperature should be 9
    EXPECT_NEAR(thermostat.getCurrentTemperature(particle_container_lc), 9, 3e-2);
    // Scale temperature to 5
    thermostat.scaleTemperature(particle_container_lc);

    // Temperature should be around 5 after scaling
    EXPECT_NEAR(thermostat.getCurrentTemperature(particle_container_lc), 5, 3e-2);
}

/*
 * Test temperature scaling heating
 */
TEST(Thermostat, TemperatureCoolingCappedStep) {
    auto thermostat = Thermostat(5, 0.5, 1, true);

    // Test with DirectSumContainer

    std::unique_ptr<ParticleContainer> particle_container_ds = std::make_unique<DirectSumContainer>();
    generateParticles(50, 50, 50, particle_container_ds);
    thermostat.setTemperature(9, particle_container_ds);

    // Initial temperature should be 9
    EXPECT_NEAR(thermostat.getCurrentTemperature(particle_container_ds), 9, 3e-2);

    // Scale temperature to 5
    thermostat.scaleTemperature(particle_container_ds);

    // Temperature should be around 8.5 after scaling since the maximum temperature change is 0.5
    EXPECT_NEAR(thermostat.getCurrentTemperature(particle_container_ds), 8.5, 3e-2);

    // Test with LinkedCellsContainer

    std::unique_ptr<ParticleContainer> particle_container_lc = createLinkedCellsContainer();
    generateParticles(50, 50, 50, particle_container_lc);
    thermostat.setTemperature(9, particle_container_lc);

    // Initial temperature should be 9
    EXPECT_NEAR(thermostat.getCurrentTemperature(particle_container_lc), 9, 3e-2);

    // Scale temperature to 5
    thermostat.scaleTemperature(particle_container_lc);

    // Temperature should be around 8.5 after scaling since the maximum temperature change is 0.5
    EXPECT_NEAR(thermostat.getCurrentTemperature(particle_container_lc), 8.5, 3e-2);
}

/*
 * Test temperature scaling keeping the temperature
 */
TEST(Thermostat, TemperatureKeeping) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(1.005, 1.05);

    auto thermostat = Thermostat(5, 0.75, 1, true);

    // Test with DirectSumContainer

    std::unique_ptr<ParticleContainer> particle_container_ds = std::make_unique<DirectSumContainer>();
    generateParticles(50, 50, 50, particle_container_ds);
    thermostat.setTemperature(5, particle_container_ds);

    // Initial temperature should be 5
    double initial_temperature_ds = thermostat.getCurrentTemperature(particle_container_ds);
    EXPECT_NEAR(initial_temperature_ds, 5, 3e-2);

    for (int i = 0; i < 10; ++i) {
        // Change each particles velocity by a random factor between 1.0 and 1.05 (should increase the temperature by a factor of at
        // most 1.1025, ~10% which from the initial temperature is around 0.5 < 0.75). The maximum temperature change is 0.75 so the
        // temperature should stay about the same.
        double factor = dist(gen);
        for (auto& it : *particle_container_ds) {
            it.setV(factor * it.getV());
        }

        EXPECT_GE(thermostat.getCurrentTemperature(particle_container_ds), initial_temperature_ds);

        thermostat.scaleTemperature(particle_container_ds);
        EXPECT_NEAR(thermostat.getCurrentTemperature(particle_container_ds), initial_temperature_ds, 3e-2);
    }

    // Test with LinkedCellsContainer

    std::unique_ptr<ParticleContainer> particle_container_lc = createLinkedCellsContainer();
    generateParticles(50, 50, 50, particle_container_lc);
    thermostat.setTemperature(5, particle_container_lc);

    // Initial temperature should be 5
    double initial_temperature_lc = thermostat.getCurrentTemperature(particle_container_lc);
    EXPECT_NEAR(initial_temperature_lc, 5, 3e-2);

    for (int i = 0; i < 10; ++i) {
        // Change each particles velocity by a random factor between 1.0 and 1.05 (should increase the temperature by a factor of at
        // most 1.1025, ~10% which from the initial temperature is around 0.5 < 0.75). The maximum temperature change is 0.75 so the
        // temperature should stay about the same.
        double factor = dist(gen);
        for (auto& it : *particle_container_lc) {
            it.setV(factor * it.getV());
        }

        EXPECT_GE(thermostat.getCurrentTemperature(particle_container_lc), initial_temperature_lc);

        thermostat.scaleTemperature(particle_container_lc);
        EXPECT_NEAR(thermostat.getCurrentTemperature(particle_container_lc), initial_temperature_lc, 3e-2);
    }
}