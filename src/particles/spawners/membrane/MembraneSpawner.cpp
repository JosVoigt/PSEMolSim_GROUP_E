#include "MembraneSpawner.h"

#include <array>
#include <memory>
#include <utility>

#include "particles/Particle.h"
#include "physics/thermostats/Thermostat.h"
#include "utils/ArrayUtils.h"

MembraneSpawner::MembraneSpawner(
    const std::array<double, 3>& lower_left_corner,
    const std::array<int, 3>& grid_dimensions, double grid_spacing, double mass,
    const std::array<double, 3>& initial_velocity, int type, double epsilon,
    double sigma, bool third_dimension, double initial_temperature,
    std::vector<std::shared_ptr<Particle>> straight_neighbours,
    std::vector<std::shared_ptr<Particle>> diagonal_neighbours)
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
      diagonal_neighbours(std::move(diagonal_neighbours)) {}

int MembraneSpawner::spawnParticles(std::vector<Particle>& particles) const {
  std::vector<std::vector<std::shared_ptr<Particle>>> grid;

  for (int i = 0; i < grid_dimensions[0]; i++) {
    //add vector
    grid.emplace_back();
    for (int j = 0; j < grid_dimensions[1]; j++) {
      std::array<double, 3> double_coord = {(double)i, (double)j, 0};
      std::array<double, 3> x = grid_spacing * double_coord + lower_left_corner;

	  std::shared_ptr<Particle> particlePointer = std::make_shared<Particle>(
          x, initial_velocity, mass, type, epsilon, sigma);

      //adjust temp
      Thermostat::setParticleTemperature(initial_temperature, *particlePointer,
                                         third_dimension ? 3 : 2);

      grid[i].push_back(particlePointer);

	  /*
	   * Particles are in a grid like this
	   *
	   * The current particle looked at is M
	   *
	   * pos y ->, pos x /\
	   *
	   *  LT T RT
	   *  LM M RM
	   *  LD D RD
	   *
	   *  If they exist we add LM, LD, D and RD
	   *  To stay consistent we add the current particles to their respective lists
	   *  This way we add every particles neighbours
	   */

      if (j == 0 && i == 0) {
        //lower left corner membrane particle -> do nothing
		//no particle we want to add exists
        continue;
      } else if (j != 0) {
        //not on the left border -> add the left particle (LD)
		  grid[i][j - 1]->addStraightNeighbour(particlePointer.get());
		  particlePointer->addStraightNeighbour(grid[i][j - 1].get());

        //check if we are on the lower border
        if (i != 0) {
			//at least one row below exists -> we can add LD and D 

			//LD
          grid[i - 1][j - 1]->addDiagonalNeighbour(particlePointer.get());
          particlePointer->addDiagonalNeighbour(grid[i - 1][j - 1].get());

			//D
          grid[i - 1][j]->addStraightNeighbour(particlePointer.get());
          particlePointer->addStraightNeighbour(grid[i - 1][j].get());

          //check if we are on the right border
          if (j + 1 != grid_dimensions[1]) {
			  //add the corner particle RD
            grid[i - 1][j + 1]->addDiagonalNeighbour(particlePointer.get());
            particlePointer->addStraightNeighbour(grid[i - 1][j + 1].get());
          }
        }

      } else {
		  //we are on the lower border but not the leftmost particle -> can add LM
        grid[i - 1][j]->addStraightNeighbour(particlePointer.get());
        particlePointer->addStraightNeighbour(grid[i - 1][j].get());
      }
    }
  }

  return grid_dimensions[0] * grid_dimensions[1];
}

size_t MembraneSpawner::getEstimatedNumberOfParticles() const {
  return static_cast<size_t>(grid_dimensions[0]) * grid_dimensions[1] *
         grid_dimensions[2];
}
