#include <vector>
#include "container/Particle.h"
#include "LinkedCellBoundary.h"

class BoundaryConditionOutflow : public LinkedCellBoundary {
	public:
		~BoundaryConditionOutflow() = default;

		BoundaryConditionOutflow (std::vector<std::vector<Particle>>& haloCellList, std::vector<std::vector<Particle>>& boundaryCellList);

		/**
		 * \brief
		 *	Executes the boundary condition on the cells attached.
		 *	This will delete all cell that are currently in halo cells.
		 *	\return
		 *	Every deleted particle
		 */
		std::vector<Particle>& executeBoundaryCondition () override;
}; 
