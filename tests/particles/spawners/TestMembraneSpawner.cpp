#include <gtest/gtest.h>

#include "particles/spawners/membrane/MembraneSpawner.h"
#include "utils/ArrayUtils.h"

/*
 * Macro to check if a point is in a list of points.
 */
#define EXPECT_CONTAINS_POS_NEAR(list, point, tol)                                                                              \
    EXPECT_TRUE(std::find_if((list).begin(), (list).end(), [&](auto& x) { return ArrayUtils::L2Norm(x - (point)) < (tol); }) != \
                (list).end());

/*
 * Test if a MembraneSpawner spawns the correct number of particles.
 */
TEST(MembraneParticleSpawner, SpawnCorrectNumberOfParticles) {
    std::array<double, 3> lower_left_corner = {0, 0, 0};
    std::array<int, 3> grid_dimensions = {3, 7, 1};
    double grid_spacing = 1;
    double mass = 1;
    std::array<double, 3> initial_velocity = {0, 0, 0};
    int type = 0;
    MembraneSpawner spawner(lower_left_corner, grid_dimensions, grid_spacing, mass, initial_velocity, type);

    std::vector<Particle> particle_container;
    spawner.spawnParticles(particle_container);

    ASSERT_EQ(particle_container.size(), 3 * 7 * 1);
}

/*
 * Test if a MembraneSpawner spawns the particles at the correct positions.
 */
TEST(MembraneParticleSpawner, SpawnParticlesAtCorrectPositions) {
    std::array<double, 3> lower_left_corner = {0, 0, 0};
    std::array<int, 3> grid_dimensions = {3, 3, 1};
    double grid_spacing = 1;
    double mass = 1;
    std::array<double, 3> initial_velocity = {0, 0, 0};
    int type = 0;
    MembraneSpawner spawner(lower_left_corner, grid_dimensions, grid_spacing, mass, initial_velocity, type);

    std::vector<Particle> particle_container;
    spawner.spawnParticles(particle_container);

    auto expected_positions =
            std::vector<std::array<double, 3>>({{0, 0, 0}, {1, 0, 0}, {2, 0, 0}, {0, 1, 0}, {0, 2, 0}, {1, 1, 0}, {1, 2, 0}, {2, 1, 0}, {2, 2, 0}});

    for (const auto& i : particle_container) {
        // check if the position of the particle is inside the expected positions
        EXPECT_CONTAINS_POS_NEAR(expected_positions, i.getX(), 1e-10)
    }
}

/*
 * Test if a MembraneParticleSpawner returns a good estimate of the actual number of particles.
 */
TEST(MembraneParticleSpawner, EstimateNumberOfParticles) {
    std::array<double, 3> lower_left = {0, 0, 0};
    double grid_spacing = 1;
    double mass = 1;
    std::array<double, 3> initial_velocity = {0, 0, 0};
    int type = 0;

    MembraneSpawner spawner2(lower_left, {2, 3, 1}, grid_spacing, mass, initial_velocity, type);
    std::vector<Particle> particle_container2;
    spawner2.spawnParticles(particle_container2);
    EXPECT_EQ(spawner2.getEstimatedNumberOfParticles(), particle_container2.size());

    MembraneSpawner spawner16(lower_left, {4, 5, 1}, grid_spacing, mass, initial_velocity, type);
    std::vector<Particle> particle_container16;
    spawner16.spawnParticles(particle_container16);
    EXPECT_EQ(spawner16.getEstimatedNumberOfParticles(), particle_container16.size());
}

/*
 * Test if a MembraneParticleSpawner initializes the neighbors of particles correctly
 */
TEST(MembraneParticleSpawner, InitializesNeighborsCorrectly) {
    std::array<double, 3> lower_left_corner = {0, 0, 0};
    std::array<int, 3> grid_dimensions = {3, 3, 1};
    double grid_spacing = 1;
    double mass = 1;
    std::array<double, 3> initial_velocity = {0, 0, 0};
    int type = 0;
    MembraneSpawner spawner(lower_left_corner, grid_dimensions, grid_spacing, mass, initial_velocity, type);

    std::vector<Particle> particle_container;
    spawner.spawnParticles(particle_container);

    //checks for the correct number of neighbors
    EXPECT_EQ(particle_container[0].getStraightNeighbours().size(), 2);
    EXPECT_EQ(particle_container[0].getDiagonalNeighbours().size(), 1);
    EXPECT_EQ(particle_container[1].getStraightNeighbours().size(), 3);
    EXPECT_EQ(particle_container[1].getDiagonalNeighbours().size(), 2);
    EXPECT_EQ(particle_container[2].getStraightNeighbours().size(), 2);
    EXPECT_EQ(particle_container[2].getDiagonalNeighbours().size(), 1);
    EXPECT_EQ(particle_container[3].getStraightNeighbours().size(), 3);
    EXPECT_EQ(particle_container[3].getDiagonalNeighbours().size(), 2);
    EXPECT_EQ(particle_container[4].getStraightNeighbours().size(), 4);
    EXPECT_EQ(particle_container[4].getDiagonalNeighbours().size(), 4);
    EXPECT_EQ(particle_container[5].getStraightNeighbours().size(), 3);
    EXPECT_EQ(particle_container[5].getDiagonalNeighbours().size(), 2);
    EXPECT_EQ(particle_container[6].getStraightNeighbours().size(), 2);
    EXPECT_EQ(particle_container[6].getDiagonalNeighbours().size(), 1);
    EXPECT_EQ(particle_container[7].getStraightNeighbours().size(), 3);
    EXPECT_EQ(particle_container[7].getDiagonalNeighbours().size(), 2);
    EXPECT_EQ(particle_container[8].getStraightNeighbours().size(), 2);
    EXPECT_EQ(particle_container[8].getDiagonalNeighbours().size(), 1);

    //checks for the correct neighbors
    bool neighbours_0 = particle_container[0].isDirectNeighbour(particle_container[1])
            && particle_container[0].isDirectNeighbour(particle_container[3])
            && particle_container[0].isDiagonalNeighbour(particle_container[4]);
    EXPECT_TRUE(neighbours_0);
    bool neighbours_1 = particle_container[1].isDirectNeighbour(particle_container[0])
            && particle_container[1].isDirectNeighbour(particle_container[2])
            && particle_container[1].isDirectNeighbour(particle_container[4])
            && particle_container[1].isDiagonalNeighbour(particle_container[3])
            && particle_container[1].isDiagonalNeighbour(particle_container[5]);
    EXPECT_TRUE(neighbours_1);
    bool neighbours_2 = particle_container[2].isDirectNeighbour(particle_container[1])
            && particle_container[2].isDirectNeighbour(particle_container[5])
            && particle_container[2].isDiagonalNeighbour(particle_container[4]);
    EXPECT_TRUE(neighbours_2);
    bool neighbours_3 = particle_container[3].isDirectNeighbour(particle_container[0])
            && particle_container[3].isDirectNeighbour(particle_container[4])
            && particle_container[3].isDirectNeighbour(particle_container[6])
            && particle_container[3].isDiagonalNeighbour(particle_container[1])
            && particle_container[3].isDiagonalNeighbour(particle_container[7]);
    EXPECT_TRUE(neighbours_3);
    bool neighbours_4 = particle_container[4].isDirectNeighbour(particle_container[1])
            && particle_container[4].isDirectNeighbour(particle_container[3])
            && particle_container[4].isDirectNeighbour(particle_container[5])
            && particle_container[4].isDirectNeighbour(particle_container[7])
            && particle_container[4].isDiagonalNeighbour(particle_container[0])
            && particle_container[4].isDiagonalNeighbour(particle_container[2])
            && particle_container[4].isDiagonalNeighbour(particle_container[6])
            && particle_container[4].isDiagonalNeighbour(particle_container[8]);
    EXPECT_TRUE(neighbours_4);
    bool neighbours_5 = particle_container[5].isDirectNeighbour(particle_container[2])
            && particle_container[5].isDirectNeighbour(particle_container[4])
            && particle_container[5].isDirectNeighbour(particle_container[8])
            && particle_container[5].isDiagonalNeighbour(particle_container[1])
            && particle_container[5].isDiagonalNeighbour(particle_container[7]);
    EXPECT_TRUE(neighbours_5);
    bool neighbours_6 = particle_container[6].isDirectNeighbour(particle_container[3])
            && particle_container[6].isDirectNeighbour(particle_container[7])
            && particle_container[6].isDiagonalNeighbour(particle_container[4]);
    EXPECT_TRUE(neighbours_6);
    bool neighbours_7 = particle_container[7].isDirectNeighbour(particle_container[4])
            && particle_container[7].isDirectNeighbour(particle_container[6])
            && particle_container[7].isDirectNeighbour(particle_container[8])
            && particle_container[7].isDiagonalNeighbour(particle_container[3])
            && particle_container[7].isDiagonalNeighbour(particle_container[5]);
    EXPECT_TRUE(neighbours_7);
    bool neighbours_8 = particle_container[8].isDirectNeighbour(particle_container[5])
            && particle_container[8].isDirectNeighbour(particle_container[7])
            && particle_container[8].isDiagonalNeighbour(particle_container[4]);
    EXPECT_TRUE(neighbours_8);

    //checks for incorrect neighbors
    bool incorrect_4 = particle_container[4].isDirectNeighbour(particle_container[0])
            && particle_container[4].isDirectNeighbour(particle_container[2])
            && particle_container[4].isDirectNeighbour(particle_container[6])
            && particle_container[4].isDirectNeighbour(particle_container[8]);
    EXPECT_FALSE(incorrect_4);
}
