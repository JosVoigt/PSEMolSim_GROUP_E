#pragma once

#include <limits>
#include <memory>

#include "particles/containers/ParticleContainer.h"

/**
 * @brief A thermostat that can be used to control the temperature of a particle container. Allows for gradual scaling of the temperature
 * towards a set target.
 */
class Thermostat {
    /**
     * @brief The target temperature for thermostat applications.
     */
    const double target_temperature;

    /**
     * @brief The maximum temperature change allowed per thermostat application.
     */
    const double max_temperature_change;

    /**
     * @brief The number of iterations between thermostat applications.
     */
    const size_t application_interval;

    /**
     * @brief The number of dimensions of the particle container that is going to be used.
     */
    const size_t dimensions;

   public:
    /**
     * @brief Construct a new Thermostat object.
     *
     * @param target_temperature The target temperature for thermostat applications.
     * @param max_temperature_change The maximum temperature change allowed per thermostat application.
     * @param application_interval The number of iterations between thermostat applications.
     * @param third_dimension True if the thermostat applies to a 3-dimensional domain, false for 2 dimensions.
     */
    Thermostat(double target_temperature, double max_temperature_change = std::numeric_limits<double>::max(),
               size_t application_interval = 1000, bool third_dimension = true);

    /**
     * @brief Get the kinetic energy of all particles in a particle container.
     *
     * @param particle_container The particle container to get the kinetic energy of.
     * @return double The total kinetic energy of all particles in the container.
     */
    double getKineticEnergy(const std::unique_ptr<ParticleContainer>& particle_container) const;

    /**
     * @brief Get the current temperature of a particle container.
     *
     * @param particle_container The particle container to get the temperature of.
     * @return double The current temperature of the particle container.
     */
    double getCurrentTemperature(const std::unique_ptr<ParticleContainer>& particle_container) const;

    /**
     * @brief Scale the temperature of a particle container towards the target temperature. Capped by the maximum temperature change.
     *
     * @param particle_container The particle container to scale the temperature of.
     */
    void scaleTemperature(const std::unique_ptr<ParticleContainer>& particle_container) const;

    /**
     * @brief Set the initial temperature of a particle container. This function sets the velocity of all particles in the container to a
     * random value according to the Maxwell-Boltzmann distribution (all previous velocities are discarded).
     * Use this function for systems with no initial velocity.
     *
     * @param new_temperature The new temperature of the particle container.
     * @param particle_container The particle container to set the initial temperature of.
     */
    void setTemperature(double new_temperature, const std::unique_ptr<ParticleContainer>& particle_container);

    /**
     * @brief Set the temperature of a particle. This method adds a random velocity to the particle according to the Maxwell-Boltzmann
     * distribution. This function can be used for particle generators since previous velocities are not discarded.
     *
     * @param new_temperature The new temperature of the particle.
     * @param particle The particle to set the initial temperature of.
     * @param dimensions The number of dimensions of the particle container.
     */
    static void setParticleTemperature(double new_temperature, Particle& particle, size_t dimensions);

    /**
     * @brief Get the target temperature of the thermostat.
     *
     * @return double The target temperature of the thermostat.
     */
    double getTargetTemperature() const;

    /**
     * @brief Get the maximum temperature change of the thermostat.
     *
     * @return double The maximum temperature change of the thermostat.
     */
    double getMaxTemperatureChange() const;

    /**
     * @brief Get the number of iterations between thermostat applications.
     *
     * @return size_t The number of iterations between thermostat applications.
     */
    size_t getApplicationInterval() const;

    /**
     * @brief Get the number of dimensions of expected to be used.
     *
     * @return size_t The number of dimensions of expected to be used.
     */
    size_t getDimensions() const;
};