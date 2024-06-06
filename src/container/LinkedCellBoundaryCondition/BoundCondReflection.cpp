#include "BoundCondReflection.h"

#include <array>
#include <vector>

BoundaryConditionReflection::BoundaryConditionReflection(
    std::vector<std::vector<Particle>>& haloCellList,
    std::vector<std::vector<Particle>>& boundaryCellList,
    std::shared_ptr<Force> forcemode, std::array<double, 3>& surface_normal,
    std::array<double, 3>& point_on_plane)
    : LinkedCellBoundary(haloCellList, boundaryCellList),
      forceMethod(forcemode),
      surface_normal(surface_normal),
      point_on_plane(point_on_plane) {}

std::vector<Particle>& BoundaryConditionReflection::executeBoundaryCondition() {}
