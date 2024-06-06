#include <memory>
#include <vector>
#include "force/Force.h"
#include "linkedCellBoundary.h"

class BoundaryConditionReflection : public LinkedCellBoundary {
	private:
		std::shared_ptr<Force> forceMethod;
	public:
		~BoundaryConditionReflection() = default;

		BoundaryConditionReflection (std::vector<Particle> haloCellList, std::vector<Particle> boundaryCellList, std::shared_ptr<Force> forcemode);
		//: LinkedCellBoundary(haloCellList, boundaryCellList), forceMethod(forcemode)

		std::vector<Particle> executeBoundaryCondition () override;
};
