#include <memory>
#include <vector>
#include "container/Particle.h"
#include "LinkedCellBoundary.h"

class BoundaryConditionOutflow : public LinkedCellBoundary {
	private:
		std::vector<std::vector<Particle>> haloCellList;
	public:
		~BoundaryConditionOutflow() = default;

		BoundaryConditionOutflow (std::vector<std::vector<Particle>>& haloCellList);

		/**
		 * \brief
		 *	Executes the boundary condition on the cells attached.
		 *	This will delete all cell that are currently in halo cells.
		 *	\return
		 *	Every deleted particle
		 */
		std::shared_ptr<std::vector<Particle>> executeBoundaryCondition () override;
}; 
