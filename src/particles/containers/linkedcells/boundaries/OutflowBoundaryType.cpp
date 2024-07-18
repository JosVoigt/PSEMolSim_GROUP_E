#include "OutflowBoundaryType.h"

#include "io/logger/Logger.h"
#include "physics/pairwiseforces/LennardJonesForce.h"
#include "utils/ArrayUtils.h"

void OutflowBoundaryType::pre(LinkedCellsContainer& container) {
    if (container.boundary_types[0] == LinkedCellsContainer::BoundaryCondition::OUTFLOW) {
        for (Cell* cell : container.left_halo_cell_references) {
            for (Particle* p : cell->getParticleReferences()) {
                container.particles.erase(std::find(container.particles.begin(), container.particles.end(), *p));
            }
        }
    }

    if (container.boundary_types[1] == LinkedCellsContainer::BoundaryCondition::OUTFLOW) {
        for (Cell* cell : container.right_halo_cell_references) {
            for (Particle* p : cell->getParticleReferences()) {
                container.particles.erase(std::find(container.particles.begin(), container.particles.end(), *p));
            }
        }
    }

    if (container.boundary_types[2] == LinkedCellsContainer::BoundaryCondition::OUTFLOW) {
        for (Cell* cell : container.bottom_halo_cell_references) {
            for (Particle* p : cell->getParticleReferences()) {
                container.particles.erase(std::find(container.particles.begin(), container.particles.end(), *p));
            }
        }
    }

    if (container.boundary_types[3] == LinkedCellsContainer::BoundaryCondition::OUTFLOW) {
        for (Cell* cell : container.top_halo_cell_references) {
            for (Particle* p : cell->getParticleReferences()) {
                container.particles.erase(std::find(container.particles.begin(), container.particles.end(), *p));
            }
        }
    }

    if (container.boundary_types[4] == LinkedCellsContainer::BoundaryCondition::OUTFLOW) {
        for (Cell* cell : container.back_halo_cell_references) {
            for (Particle* p : cell->getParticleReferences()) {
                container.particles.erase(std::find(container.particles.begin(), container.particles.end(), *p));
            }
        }
    }

    if (container.boundary_types[5] == LinkedCellsContainer::BoundaryCondition::OUTFLOW) {
        for (Cell* cell : container.front_halo_cell_references) {
            for (Particle* p : cell->getParticleReferences()) {
                container.particles.erase(std::find(container.particles.begin(), container.particles.end(), *p));
            }
        }
    }
}

void OutflowBoundaryType::applyBoundaryConditions(LinkedCellsContainer& container) {}
