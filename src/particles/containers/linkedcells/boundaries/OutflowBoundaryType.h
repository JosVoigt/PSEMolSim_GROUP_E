#pragma once

#include <array>

#include "particles/containers/linkedcells/LinkedCellsContainer.h"

class OutflowBoundaryType {
   public:
    /**
     * @brief Applies the preconditioning step for the outflow boundary condition.
     *
     * @param container The container to apply the boundary conditions to.
     *
     * This method removes all particles from the container that are in halo cells with the outflow boundary condition.
     */
    static void pre(LinkedCellsContainer& container);

    /**
     * @brief Applies the boundary conditions for the outflow boundary condition.
     *
     * @param container The container to apply the boundary conditions to.
     *
     * This method is empty, as the outflow boundary condition does not require any special treatment.
     */
    static void applyBoundaryConditions(LinkedCellsContainer& container);

   private:
    static std::array<double, 3> calculateReflectiveBoundaryForce(Particle& p, double distance, LinkedCellsContainer::BoundarySide side);
};