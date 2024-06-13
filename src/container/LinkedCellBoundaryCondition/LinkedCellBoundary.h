#include <memory>
#include <vector>
#include "container/Particle.h"

class LinkedCellBoundary {
protected:
    std::vector<std::vector<Particle>> haloCellList;
    std::vector<std::vector<Particle>> boundaryCellList;

	public:
    LinkedCellBoundary(const std::vector<std::vector<Particle>>& haloCellList, const std::vector<std::vector<Particle>>& boundaryCellList);

    virtual ~LinkedCellBoundary();

	virtual std::shared_ptr<std::vector<Particle>> executeBoundaryCondition ();
};

