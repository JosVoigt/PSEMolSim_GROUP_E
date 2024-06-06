#include "BoundCondReflection.h"

#include <array>
#include <memory>
#include <vector>

#include "container/Particle.h"
#include "utils/ArrayUtils.h"

BoundaryConditionReflection::BoundaryConditionReflection(
    std::vector<std::vector<Particle>>& haloCellList,
    std::vector<std::vector<Particle>>& boundaryCellList,
    std::shared_ptr<Force> forcemode, std::array<double, 3>& surface_normal,
    std::array<double, 3>& point_on_plane)
    : LinkedCellBoundary(haloCellList, boundaryCellList),
      forceMethod(forcemode),
      surface_normal(surface_normal),
      point_on_plane(point_on_plane) {}

std::shared_ptr<std::vector<Particle>>
BoundaryConditionReflection::executeBoundaryCondition() {
  for (auto& cell : haloCellList) {
    for (Particle& p : cell) {
      // calculate distance
      std::array<double, 3> p_x = p.getX();
      std::array<double, 3> distance_array =
          (point_on_plane - p_x) * surface_normal;
      std::array<double, 3> halo_x = p_x + 2 * distance_array;

      std::array<double, 3> zero = {0, 0, 0};

      Particle halo = Particle(halo_x, zero, p.getM(), -1);

      std::array<double, 3> force = forceMethod->calculateForce(p, halo);

      std::array<double, 3> forceToWall = force * surface_normal;

      if (forceToWall[0] + forceToWall[1] + forceToWall[2] > 0) {
        p.addF(force);
      }
    }
  }
  std::shared_ptr<std::vector<Particle>> removed =
      std::shared_ptr<std::vector<Particle>>(new std::vector<Particle>());
	return removed;
}
