#pragma once

#include <array>

#include "particles/containers/linkedcells/LinkedCellsContainer.h"

class PeriodicBoundaryType {
   public:
    /**
     * @brief Applies the preconditioning step for the periodic boundary condition.
     *
     * @param container The container to apply the boundary conditions to.
     *
     * This method teleports all particles that are in halo cells with the periodic boundary condition to the opposite side of the
     * container.
     */
    static void pre(LinkedCellsContainer& container);

    /**
     * @brief Applies the boundary conditions for the periodic boundary condition.
     *
     * @param container The container to apply the boundary conditions to.
     *
     * This method adds all the Halo particles for the periodic boundary to the respective side of the container.
     */
    static void applyBoundaryConditions(LinkedCellsContainer& container);

   private:
    /**
     * @brief Adds the periodic halo particles for the given side of the container to the other side of the container.
     *
     * @param container The container to add the particles to.
     * @param side_cell_references The cell references of the halo cells for the given side.
     * @param offset The offset to the opposite side of the container.
     */
    static void addPeriodicHaloParticlesForSide(LinkedCellsContainer& container, const std::vector<Cell*>& side_cell_references,
                                                const std::array<double, 3>& offset);

    /**
     * @brief Adds the periodic halo particles for the given edge of the container to the diagonally opposite edge of the container.
     * @param container The container to add the particles to.
     * @param free_dimension The free dimension of the edge.
     * @param offset The offset to the opposite edge of the container.
     */
    static void addPeriodicHaloParticlesForEdge(LinkedCellsContainer& container, int free_dimension, const std::array<double, 3>& offset);

    /**
     * @brief Adds the periodic halo particles for the given corner of the container to the diagonally opposite corner of the container.
     * @param container The container to add the particles to.
     * @param offset The offset to the opposite corner of the container.
     */
    static void addPeriodicHaloParticlesForCorner(LinkedCellsContainer& container, const std::array<double, 3>& offset);
};
