#pragma once

#include <list>
#include <vector>

#include "Particle.h"

class ParticleContainerInterface {
 public:
  /**
     * \brief
     * Destructor
     */
  virtual ~ParticleContainerInterface() = default;

  ParticleContainerInterface(std::list<Particle> &init) {
     initialize(init);
  };

  void initialize(std::list<Particle> &particleList) {
     for (Particle &p : particleList) {
       insertParticle(p);
     }
  }

  virtual void insertParticle(const Particle &particle) = 0;

  /**
     * \brief
     * Returns the list of Particles in case they are needed for preprocessing purposes
     */
  virtual std::vector<Particle> preprocessParticles() = 0;

  /**
     * \brief
     * Returns a list of all particles in the container
     */
  virtual std::vector<Particle> retrieveRelevantParticles(
      Particle& particle) = 0;

  /**
     * \brief
     * Updates the location and state of the particles in the container
     */
  virtual void updateParticles() = 0;

  [[nodiscard]] virtual std::size_t size() const = 0;

  virtual std::vector<Particle>::iterator begin() = 0;

  virtual std::vector<Particle>::iterator end() = 0;
};
