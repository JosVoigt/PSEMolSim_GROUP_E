#include "PeriodicBoundaryType.h"

#include "io/logger/Logger.h"
#include "physics/pairwiseforces/LennardJonesForce.h"
#include "utils/ArrayUtils.h"

void PeriodicBoundaryType::pre(LinkedCellsContainer& container) {
    if (container.boundary_types[0] == LinkedCellsContainer::LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        for (Cell* cell : container.left_halo_cell_references) {
            for (Particle* p : cell->getParticleReferences()) {
                p->setX(p->getX() + std::array<double, 3>{container.domain_size[0], 0, 0});
            }
        }
    }

    if (container.boundary_types[1] == LinkedCellsContainer::LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        for (Cell* cell : container.right_halo_cell_references) {
            for (Particle* p : cell->getParticleReferences()) {
                p->setX(p->getX() + std::array<double, 3>{-container.domain_size[0], 0, 0});
            }
        }
    }

    if (container.boundary_types[2] == LinkedCellsContainer::LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        for (Cell* cell : container.bottom_halo_cell_references) {
            for (Particle* p : cell->getParticleReferences()) {
                p->setX(p->getX() + std::array<double, 3>{0, container.domain_size[1], 0});
            }
        }
    }

    if (container.boundary_types[3] == LinkedCellsContainer::LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        for (Cell* cell : container.top_halo_cell_references) {
            for (Particle* p : cell->getParticleReferences()) {
                p->setX(p->getX() + std::array<double, 3>{0, -container.domain_size[1], 0});
            }
        }
    }

    if (container.boundary_types[4] == LinkedCellsContainer::LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        for (Cell* cell : container.back_halo_cell_references) {
            for (Particle* p : cell->getParticleReferences()) {
                p->setX(p->getX() + std::array<double, 3>{0, 0, container.domain_size[2]});
            }
        }
    }

    if (container.boundary_types[5] == LinkedCellsContainer::LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        for (Cell* cell : container.front_halo_cell_references) {
            for (Particle* p : cell->getParticleReferences()) {
                p->setX(p->getX() + std::array<double, 3>{0, 0, -container.domain_size[2]});
            }
        }
    }
}

void PeriodicBoundaryType::applyBoundaryConditions(LinkedCellsContainer& container) {
    // Add Halo Particles for each side of the domain
    if (container.boundary_types[0] == LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        addPeriodicHaloParticlesForSide(container, container.left_boundary_cell_references, {container.domain_size[0], 0, 0});
    }

    if (container.boundary_types[1] == LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        addPeriodicHaloParticlesForSide(container, container.right_boundary_cell_references, {-container.domain_size[0], 0, 0});
    }

    if (container.boundary_types[2] == LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        addPeriodicHaloParticlesForSide(container, container.bottom_boundary_cell_references, {0, container.domain_size[1], 0});
    }

    if (container.boundary_types[3] == LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        addPeriodicHaloParticlesForSide(container, container.top_boundary_cell_references, {0, -container.domain_size[1], 0});
    }

    if (container.boundary_types[4] == LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        addPeriodicHaloParticlesForSide(container, container.back_boundary_cell_references, {0, 0, container.domain_size[2]});
    }

    if (container.boundary_types[5] == LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        addPeriodicHaloParticlesForSide(container, container.front_boundary_cell_references, {0, 0, -container.domain_size[2]});
    }

    // Add Halo Particles for each edge of the domain
    if (container.boundary_types[0] == LinkedCellsContainer::BoundaryCondition::PERIODIC &&
        container.boundary_types[2] == LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        addPeriodicHaloParticlesForEdge(container, 2, {container.domain_size[0], container.domain_size[1], 0});
    }

    if (container.boundary_types[0] == LinkedCellsContainer::BoundaryCondition::PERIODIC &&
        container.boundary_types[3] == LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        addPeriodicHaloParticlesForEdge(container, 2, {container.domain_size[0], -container.domain_size[1], 0});
    }

    if (container.boundary_types[1] == LinkedCellsContainer::BoundaryCondition::PERIODIC &&
        container.boundary_types[2] == LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        addPeriodicHaloParticlesForEdge(container, 2, {-container.domain_size[0], container.domain_size[1], 0});
    }

    if (container.boundary_types[1] == LinkedCellsContainer::BoundaryCondition::PERIODIC &&
        container.boundary_types[3] == LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        addPeriodicHaloParticlesForEdge(container, 2, {-container.domain_size[0], -container.domain_size[1], 0});
    }

    if (container.boundary_types[0] == LinkedCellsContainer::BoundaryCondition::PERIODIC &&
        container.boundary_types[4] == LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        addPeriodicHaloParticlesForEdge(container, 1, {container.domain_size[0], 0, container.domain_size[2]});
    }

    if (container.boundary_types[0] == LinkedCellsContainer::BoundaryCondition::PERIODIC &&
        container.boundary_types[5] == LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        addPeriodicHaloParticlesForEdge(container, 1, {container.domain_size[0], 0, -container.domain_size[2]});
    }

    if (container.boundary_types[1] == LinkedCellsContainer::BoundaryCondition::PERIODIC &&
        container.boundary_types[4] == LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        addPeriodicHaloParticlesForEdge(container, 1, {-container.domain_size[0], 0, container.domain_size[2]});
    }

    if (container.boundary_types[1] == LinkedCellsContainer::BoundaryCondition::PERIODIC &&
        container.boundary_types[5] == LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        addPeriodicHaloParticlesForEdge(container, 1, {-container.domain_size[0], 0, -container.domain_size[2]});
    }

    if (container.boundary_types[2] == LinkedCellsContainer::BoundaryCondition::PERIODIC &&
        container.boundary_types[4] == LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        addPeriodicHaloParticlesForEdge(container, 0, {0, container.domain_size[1], container.domain_size[2]});
    }

    if (container.boundary_types[2] == LinkedCellsContainer::BoundaryCondition::PERIODIC &&
        container.boundary_types[5] == LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        addPeriodicHaloParticlesForEdge(container, 0, {0, container.domain_size[1], -container.domain_size[2]});
    }

    if (container.boundary_types[3] == LinkedCellsContainer::BoundaryCondition::PERIODIC &&
        container.boundary_types[4] == LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        addPeriodicHaloParticlesForEdge(container, 0, {0, -container.domain_size[1], container.domain_size[2]});
    }

    if (container.boundary_types[3] == LinkedCellsContainer::BoundaryCondition::PERIODIC &&
        container.boundary_types[5] == LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        addPeriodicHaloParticlesForEdge(container, 0, {0, -container.domain_size[1], -container.domain_size[2]});
    }

    // Add Halo Particles for each corner of the domain
    if (container.boundary_types[0] == LinkedCellsContainer::BoundaryCondition::PERIODIC &&
        container.boundary_types[2] == LinkedCellsContainer::BoundaryCondition::PERIODIC &&
        container.boundary_types[4] == LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        addPeriodicHaloParticlesForCorner(container, {container.domain_size[0], container.domain_size[1], container.domain_size[2]});
    }

    if (container.boundary_types[0] == LinkedCellsContainer::BoundaryCondition::PERIODIC &&
        container.boundary_types[2] == LinkedCellsContainer::BoundaryCondition::PERIODIC &&
        container.boundary_types[5] == LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        addPeriodicHaloParticlesForCorner(container, {container.domain_size[0], container.domain_size[1], -container.domain_size[2]});
    }

    if (container.boundary_types[0] == LinkedCellsContainer::BoundaryCondition::PERIODIC &&
        container.boundary_types[3] == LinkedCellsContainer::BoundaryCondition::PERIODIC &&
        container.boundary_types[4] == LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        addPeriodicHaloParticlesForCorner(container, {container.domain_size[0], -container.domain_size[1], container.domain_size[2]});
    }

    if (container.boundary_types[0] == LinkedCellsContainer::BoundaryCondition::PERIODIC &&
        container.boundary_types[3] == LinkedCellsContainer::BoundaryCondition::PERIODIC &&
        container.boundary_types[5] == LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        addPeriodicHaloParticlesForCorner(container, {container.domain_size[0], -container.domain_size[1], -container.domain_size[2]});
    }

    if (container.boundary_types[1] == LinkedCellsContainer::BoundaryCondition::PERIODIC &&
        container.boundary_types[2] == LinkedCellsContainer::BoundaryCondition::PERIODIC &&
        container.boundary_types[4] == LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        addPeriodicHaloParticlesForCorner(container, {-container.domain_size[0], container.domain_size[1], container.domain_size[2]});
    }

    if (container.boundary_types[1] == LinkedCellsContainer::BoundaryCondition::PERIODIC &&
        container.boundary_types[2] == LinkedCellsContainer::BoundaryCondition::PERIODIC &&
        container.boundary_types[5] == LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        addPeriodicHaloParticlesForCorner(container, {-container.domain_size[0], container.domain_size[1], -container.domain_size[2]});
    }

    if (container.boundary_types[1] == LinkedCellsContainer::BoundaryCondition::PERIODIC &&
        container.boundary_types[3] == LinkedCellsContainer::BoundaryCondition::PERIODIC &&
        container.boundary_types[4] == LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        addPeriodicHaloParticlesForCorner(container, {-container.domain_size[0], -container.domain_size[1], container.domain_size[2]});
    }

    if (container.boundary_types[1] == LinkedCellsContainer::BoundaryCondition::PERIODIC &&
        container.boundary_types[3] == LinkedCellsContainer::BoundaryCondition::PERIODIC &&
        container.boundary_types[5] == LinkedCellsContainer::BoundaryCondition::PERIODIC) {
        addPeriodicHaloParticlesForCorner(container, {-container.domain_size[0], -container.domain_size[1], -container.domain_size[2]});
    }
}

void PeriodicBoundaryType::addPeriodicHaloParticlesForSide(LinkedCellsContainer& container, const std::vector<Cell*>& side_cell_references,
                                                           const std::array<double, 3>& offset) {
    for (Cell* cell : side_cell_references) {
        for (Particle* p : cell->getParticleReferences()) {
            Particle ghost_particle = Particle(*p);
            ghost_particle.setX(p->getX() + offset);
            container.addParticle(ghost_particle);
        }
    }
}

void PeriodicBoundaryType::addPeriodicHaloParticlesForEdge(LinkedCellsContainer& container, int free_dimension,
                                                           const std::array<double, 3>& offset) {
    std::array<int, 3> running_array{0, 0, 0};
    running_array[0] = (offset[0] < 0) ? container.domain_num_cells[0] - 1 : 0;
    running_array[1] = (offset[1] < 0) ? container.domain_num_cells[1] - 1 : 0;
    running_array[2] = (offset[2] < 0) ? container.domain_num_cells[2] - 1 : 0;

    for (int c = 0; c < container.domain_num_cells[2]; ++c) {
        Cell* cell = &container.cells.at(container.cellCoordToCellIndex(running_array[0], running_array[1], running_array[2]));
        for (Particle* p : cell->getParticleReferences()) {
            Particle ghost_particle = Particle(*p);
            ghost_particle.setX(p->getX() + offset);
            container.addParticle(ghost_particle);
        }
        running_array[free_dimension] += 1;
    }
}

void PeriodicBoundaryType::addPeriodicHaloParticlesForCorner(LinkedCellsContainer& container, const std::array<double, 3>& offset) {
    std::array<int, 3> cell_coords{0, 0, 0};
    cell_coords[0] = (offset[0] < 0) ? container.domain_num_cells[0] - 1 : 0;
    cell_coords[1] = (offset[1] < 0) ? container.domain_num_cells[1] - 1 : 0;
    cell_coords[2] = (offset[2] < 0) ? container.domain_num_cells[2] - 1 : 0;

    Cell* cell = &container.cells.at(container.cellCoordToCellIndex(cell_coords[0], cell_coords[1], cell_coords[2]));
    for (Particle* p : cell->getParticleReferences()) {
        Particle ghost_particle = Particle(*p);
        ghost_particle.setX(p->getX() + offset);
        container.addParticle(ghost_particle);
    }
}
