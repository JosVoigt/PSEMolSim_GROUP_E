
#include <memory>
#include <vector>
#include "container/Particle.h"
class LinkedCellBoundary {
	public:
		virtual ~LinkedCellBoundary();

		virtual std::shared_ptr<std::vector<Particle>> executeBoundaryCondition ();
};
