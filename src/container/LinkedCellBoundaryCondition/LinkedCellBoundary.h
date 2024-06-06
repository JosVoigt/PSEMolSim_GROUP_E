
#include <vector>
#include "container/Particle.h"
class LinkedCellBoundary {
	public:
		virtual ~LinkedCellBoundary();

		LinkedCellBoundary (std::vector<std::vector<Particle>>& haloCellList_, std::vector<std::vector<Particle>>& boundaryCellList_) : haloCellList(haloCellList_), boundaryCellList(boundaryCellList_) {};

		std::vector<std::vector<Particle>> haloCellList;
		std::vector<std::vector<Particle>> boundaryCellList;

		virtual std::vector<Particle>& executeBoundaryCondition ();
};
