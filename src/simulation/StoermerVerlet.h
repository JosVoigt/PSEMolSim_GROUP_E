#pragma once

#include <memory>

#include "container/Particle.h"
#include "container/ParticleContainerBasic.h"
#include "container/ParticleContainerInterface.h"
#include "force/Force.h"
#include "utils/ArrayUtils.h"
#include "utils/Parser.h"

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
void calculateX(std::vector<Particle>& particleList, double dt, double dt_sq);

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
void calculateV(std::vector<Particle>& particleList, double dt);

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
void calculateF(std::vector<Particle>& particleList, std::shared_ptr<ParticleContainerInterface>& container,
                std::shared_ptr<PairwiseForce> method);

void calculateGeneralF(std::vector<Particle>& particleList,
                       std::shared_ptr<GeneralForce>& force);
