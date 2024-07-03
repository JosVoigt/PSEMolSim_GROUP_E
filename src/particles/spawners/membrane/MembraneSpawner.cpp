#include "MembraneSpawner.h"

#include <utility>

#include "particles/Particle.h"
#include "physics/thermostats/Thermostat.h"
#include "utils/ArrayUtils.h"

MembraneSpawner::MembraneSpawner(const std::array<double, 3>& lower_left_corner, const std::array<int, 3>& grid_dimensions, double grid_spacing,
                             double mass, const std::array<double, 3>& initial_velocity, int type, double epsilon, double sigma,
                             bool third_dimension, double initial_temperature, std::vector<std::shared_ptr<Particle>> straight_neighbours, std::vector<std::shared_ptr<Particle>> diagonal_neighbours)
        : lower_left_corner(lower_left_corner),
          grid_dimensions(grid_dimensions),
          grid_spacing(grid_spacing),
          mass(mass),
          type(type),
          epsilon(epsilon),
          sigma(sigma),
          initial_velocity(initial_velocity),
          third_dimension(third_dimension),
          initial_temperature(initial_temperature),
          straight_neighbours(std::move(straight_neighbours)),
          diagonal_neighbours(std::move(diagonal_neighbours)){}

int MembraneSpawner::spawnParticles(std::vector<Particle>& particles) const {
    std::vector<std::vector<std::vector<std::shared_ptr<Particle>>>> grid(grid_dimensions[0],
                                                                          std::vector<std::vector<std::shared_ptr<Particle>>>(
                                                                                  grid_dimensions[1],
                                                                                  std::vector<std::shared_ptr<Particle>>(
                                                                                          grid_dimensions[2])));

    particles.reserve(particles.size() + getEstimatedNumberOfParticles());
    for (int i = 0; i < grid_dimensions[0]; i++) {
        for (int j = 0; j < grid_dimensions[1]; j++) {
            for (int k = 0; k < grid_dimensions[2]; k++) {
                const auto grid_pos = std::array<double, 3>{static_cast<double>(i), static_cast<double>(j),
                                                            static_cast<double>(k)};

                const auto x = lower_left_corner + grid_spacing * grid_pos;

                Particle particle(x, initial_velocity, mass, type, epsilon, sigma);
                Thermostat::setParticleTemperature(initial_temperature, particle, third_dimension ? 3 : 2);
                particles.push_back(std::move(particle));

                grid[i][j][k] = std::make_shared<Particle>(particle);
            }
        }
    }

    for (int i = 0; i < grid_dimensions[0]; i++) {
        for (int j = 0; j < grid_dimensions[1]; j++) {

                //left particle
                if (i > 0) {
                    grid[i][j][0]->addStraightNeighbour(grid[i - 1][j][0]);
                }
                //right particle
                if (i < grid_dimensions[0] - 1) {
                    grid[i][j][0]->addStraightNeighbour(grid[i + 1][j][0]);
                }
                //upper particle
                if (j < grid_dimensions[1] - 1) {
                    grid[i][j][0]->addStraightNeighbour(grid[i][j + 1][0]);
                }
                //bottom particle
                if (j > 0) {
                    grid[i][j][0]->addStraightNeighbour(grid[i][j - 1][0]);
                }

                //left upper particle
                if (i > 0 && j < grid_dimensions[1] - 1) {
                    grid[i][j][0]->addDiagonalNeighbour(grid[i - 1][j + 1][0]);
                }
                //left bottom particle
                if (i > 0 && j > 0) {
                    grid[i][j][0]->addDiagonalNeighbour(grid[i - 1][j - 1][0]);
                }
                //right upper particle
                if (i < grid_dimensions[0] - 1 && j < grid_dimensions[1] - 1) {
                    grid[i][j][0]->addDiagonalNeighbour(grid[i + 1][j + 1][0]);
                }
                //right bottom particle
                if (i < grid_dimensions[0] - 1 && j > 0) {
                    grid[i][j][0]->addDiagonalNeighbour(grid[i + 1][j - 1][0]);
                }

        }
    }

    return grid_dimensions[0] * grid_dimensions[1] * grid_dimensions[2];
}

size_t MembraneSpawner::getEstimatedNumberOfParticles() const {
    return static_cast<size_t>(grid_dimensions[0]) * grid_dimensions[1] * grid_dimensions[2];
}