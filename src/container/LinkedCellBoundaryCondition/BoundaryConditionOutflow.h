#include <vector>
#include "container/Particle.h"
#include "LinkedCellBoundary.h"

class BoundaryConditionOutflow : public LinkedCellBoundary {
	public:
		~BoundaryConditionOutflow() = default;

		BoundaryConditionOutflow (std::vector<Particle> haloCellList, std::vector<Particle> boundaryCellList);

		std::vector<Particle> executeBoundaryCondition () override;
}; 
