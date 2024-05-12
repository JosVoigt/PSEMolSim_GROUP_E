#pragma once

#include "container/Particle.h"
#include "container/ParticleContainer.h"
#include "force/Force.h"
#include "utils/ArrayUtils.h"

/** \brief
 *  Calculates the new positions according to Störmer-Verlet
 *
 *  \param container
 *  The ParticleContainer containing all particles the position shall be
 *  calculated
 *
 *  \param dt
 *  The time difference between iteration steps
 *
 *  \param dt_sq
 *  The time difference dt squared
 *  If dt_sq is not equal to dt^2 behaviour is undefined
 *
 */
void calculateX(ParticleContainer &container, const double dt,
                const double dt_sq) {
    for (Particle &p : container) {
        std::array<double, 3> res =
            (dt * p.getV()) + (dt_sq / (2 * p.getM())) * p.getF();
        p.addX(res);
    }
}

/** \brief
 *  Calculates the new velocities according to Störmer-Verlet
 *
 *  \param container
 *  The ParticleContainer containing all particles the velocity shall be
 *  calculated
 *
 *  \param dt
 *  The time difference between iteration steps
 *
 */
void calculateV(ParticleContainer &container, double dt) {
    for (Particle &p : container) {
        std::array<double, 3> res =
            (dt / (2 * p.getM())) * (p.getF() + p.getOldF());
        p.addV(res);
    }
}
/** \brief
 *  Calculates the new force for all particles in the particle container with
 * the selected method
 *
 *  \param container
 *  The ParticleContainer containing all particles the force shall be calculated
 *
 *  \param method
 *  The force equation that describes the system
 *
 */
void calculateF(ParticleContainer &container,
                const std::shared_ptr<Force> method) {
    for (Particle &p : container) {
        p.nextIteration();
    }

    for (auto iterator = container.begin(); iterator != container.end();
         iterator++) {
        for (auto inner = iterator + 1; inner != container.end(); inner++) {
            std::array<double, 3> f = method->calculateForce(*iterator, *inner);
            (*iterator).addF(f);
            std::array<double, 3> invF = -1 * f;
            (*inner).addF(invF);
        }
    }
}