#pragma once

#include <memory>
#include <vector>
#include "container/Particle.h"

class LinkedCellBoundary {
	public:
    virtual ~LinkedCellBoundary() = default;

	virtual std::shared_ptr<std::vector<Particle>> executeBoundaryCondition () = 0;
};

