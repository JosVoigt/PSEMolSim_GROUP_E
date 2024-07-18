#pragma once

#include <array>

#include "particles/containers/linkedcells/LinkedCellsContainer.h"

class ReflectiveBoundaryType {
   public:
    /**
     * @brief Applies the preconditioning step for the reflective boundary condition.
     *
     * @param container The container to apply the boundary conditions to.
     *
     * This method is empty, as the reflective boundary condition does not require any special treatment.
     */
    static void pre(LinkedCellsContainer& container);

    /**
     * @brief Applies the boundary conditions for the reflective boundary condition.
     *
     * @param container The container to apply the boundary conditions to.
     *
     * This method adds the force of the reflective boundary to all particles that are in halo cells with the reflective boundary condition.
     */
    static void applyBoundaryConditions(LinkedCellsContainer& container);

   private:
    /**
     * @brief Calculates the force of the reflective boundary for the given particle.
     *
     * @param p The particle to calculate the force for.
     * @param distance The distance of the particle to the boundary.
     * @param side The side of the boundary.
     *
     * @return The force of the reflective boundary for the given particle.
     */
    static std::array<double, 3> calculateReflectiveBoundaryForce(Particle& p, double distance, LinkedCellsContainer::BoundarySide side);
};