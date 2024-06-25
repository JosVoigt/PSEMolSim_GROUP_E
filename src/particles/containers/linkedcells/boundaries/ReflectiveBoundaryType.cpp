#include "ReflectiveBoundaryType.h"

#include "io/logger/Logger.h"
#include "physics/pairwiseforces/LennardJonesForce.h"
#include "utils/ArrayUtils.h"

void ReflectiveBoundaryType::pre(LinkedCellsContainer& container){

};

void ReflectiveBoundaryType::applyBoundaryConditions(LinkedCellsContainer& container) {
    if (container.boundary_types[0] == LinkedCellsContainer::BoundaryCondition::REFLECTIVE) {
        for (Cell* cell : container.left_boundary_cell_references) {
            for (Particle* p : cell->getParticleReferences()) {
                double distance = p->getX()[0];
                p->setF(p->getF() + calculateReflectiveBoundaryForce(*p, distance, LinkedCellsContainer::BoundarySide::LEFT));
            }
        }
    }

    if (container.boundary_types[1] == LinkedCellsContainer::BoundaryCondition::REFLECTIVE) {
        for (Cell* cell : container.right_boundary_cell_references) {
            for (Particle* p : cell->getParticleReferences()) {
                double distance = container.domain_size[0] - p->getX()[0];
                p->setF(p->getF() + calculateReflectiveBoundaryForce(*p, distance, LinkedCellsContainer::BoundarySide::RIGHT));
            }
        }
    }

    if (container.boundary_types[2] == LinkedCellsContainer::BoundaryCondition::REFLECTIVE) {
        for (Cell* cell : container.bottom_boundary_cell_references) {
            for (Particle* p : cell->getParticleReferences()) {
                double distance = p->getX()[1];
                p->setF(p->getF() + calculateReflectiveBoundaryForce(*p, distance, LinkedCellsContainer::BoundarySide::BOTTOM));
            }
        }
    }

    if (container.boundary_types[3] == LinkedCellsContainer::BoundaryCondition::REFLECTIVE) {
        for (Cell* cell : container.top_boundary_cell_references) {
            for (Particle* p : cell->getParticleReferences()) {
                double distance = container.domain_size[1] - p->getX()[1];
                p->setF(p->getF() + calculateReflectiveBoundaryForce(*p, distance, LinkedCellsContainer::BoundarySide::TOP));
            }
        }
    }

    if (container.boundary_types[4] == LinkedCellsContainer::BoundaryCondition::REFLECTIVE) {
        for (Cell* cell : container.back_boundary_cell_references) {
            for (Particle* p : cell->getParticleReferences()) {
                double distance = p->getX()[2];
                p->setF(p->getF() + calculateReflectiveBoundaryForce(*p, distance, LinkedCellsContainer::BoundarySide::BACK));
            }
        }
    }

    if (container.boundary_types[5] == LinkedCellsContainer::BoundaryCondition::REFLECTIVE) {
        for (Cell* cell : container.front_boundary_cell_references) {
            for (Particle* p : cell->getParticleReferences()) {
                double distance = container.domain_size[2] - p->getX()[2];
                p->setF(p->getF() + calculateReflectiveBoundaryForce(*p, distance, LinkedCellsContainer::BoundarySide::FRONT));
            }
        }
    }
}

std::array<double, 3> ReflectiveBoundaryType::calculateReflectiveBoundaryForce(Particle& p, double distance,
                                                                               LinkedCellsContainer::BoundarySide side) {
    LennardJonesForce force = LennardJonesForce();

    if (2 * distance >= std::pow(2, 1.0 / 6) * p.getSigma()) {
        return {0, 0, 0};
    }

    Particle ghost_particle = Particle(p);
    ghost_particle.setX(p.getX() - std::array<double, 3>{2 * distance, 0, 0});

    auto force_vector_left_side = force.calculateForce(p, ghost_particle);

    switch (side) {
        case LinkedCellsContainer::BoundarySide::LEFT:
            return force_vector_left_side;
        case LinkedCellsContainer::BoundarySide::RIGHT:
            return {-force_vector_left_side[0], 0, 0};
        case LinkedCellsContainer::BoundarySide::BOTTOM:
            return {0, force_vector_left_side[0], 0};
        case LinkedCellsContainer::BoundarySide::TOP:
            return {0, -force_vector_left_side[0], 0};
        case LinkedCellsContainer::BoundarySide::BACK:
            return {0, 0, force_vector_left_side[0]};
        case LinkedCellsContainer::BoundarySide::FRONT:
            return {0, 0, -force_vector_left_side[0]};
    }

    Logger::logger->error("Faulty reflective boundary condition");
    return {0, 0, 0};
}