#include "BoundaryConditionOutflow.h"
#include <vector>
#include "container/Particle.h"


BoundaryConditionOutflow::BoundaryConditionOutflow (std::vector<Particle> haloCellList, std::vector<Particle> boundaryCellList) : LinkedCellBoundary(haloCellList, boundaryCellList){}


std::vector<Particle> BoundaryConditionOutflow::executeBoundaryCondition () {
	std::vector<Particle> removed;

	//move all particles to remove
	for (Particle p : haloCellList) {
		removed.push_back(p);
	}

	//delete all halo cell particles
	haloCellList.clear();

	//return all the removed particles
	return removed;
}
