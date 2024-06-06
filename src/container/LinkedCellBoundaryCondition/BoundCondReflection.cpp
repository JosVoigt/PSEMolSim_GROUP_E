#include "BoundCondReflection.h"

#include <vector>
#include <array>
#include "container/Particle.h"

BoundaryConditionReflection::BoundaryConditionReflection(
    std::vector<Particle> haloCellList, std::vector<Particle> boundaryCellList,
    std::shared_ptr<Force> forcemode)
    : LinkedCellBoundary(haloCellList, boundaryCellList),
      forceMethod(forcemode) {}

std::vector<Particle> BoundaryConditionReflection::executeBoundaryCondition() {
  for (Particle p : boundaryCellList) {
    // generate halo particle
    Particle halo;

    // calculate force
    std::array<double> force = forceMethod->calculateForce(p, halo);

    // check if force is posititve
    // calculate angle probably best idea?
    // add if positive and particle gets away from boundary
  }

  std::vector<Particle> removed;
  // return empty vector as nothing got removed
  return removed;
}
