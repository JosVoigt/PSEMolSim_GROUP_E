#include "BoundaryConditionOutflow.h"

#include <vector>

#include "container/Particle.h"

BoundaryConditionOutflow::BoundaryConditionOutflow(
    std::vector<std::vector<Particle>>& haloCellList,
    std::vector<std::vector<Particle>>& boundaryCellList)
    : LinkedCellBoundary(haloCellList, boundaryCellList) {}

std::vector<Particle>& BoundaryConditionOutflow::executeBoundaryCondition() {
  std::vector<Particle> removed = std::vector<Particle>();

  // remove from all halo cells
  for (auto &cell : haloCellList) {
    for (Particle &p : cell) {
      removed.push_back(p);
    }
    // delete all halo cell particles
	cell.clear();
  }

  // return all the removed particles
  return removed;
}
