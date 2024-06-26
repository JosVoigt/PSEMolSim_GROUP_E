#include "BoundaryConditionOutflow.h"

#include <vector>

#include "container/Particle.h"

BoundaryConditionOutflow::BoundaryConditionOutflow(
    std::vector<std::vector<Particle>>& haloCellList)
    : haloCellList(haloCellList) {}

std::shared_ptr<std::vector<Particle>>
BoundaryConditionOutflow::executeBoundaryCondition() {
  std::shared_ptr<std::vector<Particle>> removed =
      std::shared_ptr<std::vector<Particle>>(new std::vector<Particle>());
  // remove from all halo cells
  for (auto& cell : haloCellList) {
    for (Particle& p : cell) {
      removed->push_back(p);
    }
    // delete all halo cell particles
    cell.clear();
  }

  // return all the removed particles
  return removed;
}
