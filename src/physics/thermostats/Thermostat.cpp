#include "Thermostat.h"

#include <cmath>

#include "io/logger/Logger.h"
#include "utils/ArrayUtils.h"
#include "utils/MaxwellBoltzmannDistribution.h"

Thermostat::Thermostat(double target_temperature, double max_temperature_change, size_t application_interval, bool third_dimension)
    : target_temperature(target_temperature),
      max_temperature_change(max_temperature_change),
      application_interval(application_interval),
      dimensions(third_dimension ? 3 : 2) {}

double Thermostat::getKineticEnergy(const std::unique_ptr<ParticleContainer>& particle_container) const {
    double total_kinetic_energy = 0;
    for (auto it = particle_container->begin(); it != particle_container->end(); ++it) {
        std::array<double, 3> v = it->getV();
        total_kinetic_energy += it->getM() * (v[0] * v[0] + v[1] * v[1] + v[2] * v[2]) * 0.5;
    }
    return total_kinetic_energy;
}

double Thermostat::getCurrentTemperature(const std::unique_ptr<ParticleContainer>& particle_container) const {
    return 2 * getKineticEnergy(particle_container) / (dimensions * particle_container->size());
}

void Thermostat::scaleTemperature(const std::unique_ptr<ParticleContainer>& particle_container) const {
    const double current_temperature = getCurrentTemperature(particle_container);
    const double temperature_change = std::min(std::abs(target_temperature - current_temperature), max_temperature_change);
    const double new_temperature = current_temperature + temperature_change * (target_temperature > current_temperature ? 1 : -1);

    const double scaling_factor = std::sqrt(new_temperature / current_temperature);

    for (auto it = particle_container->begin(); it != particle_container->end(); ++it) {
        it->setV(scaling_factor * it->getV());
    }
}

void Thermostat::setTemperature(double new_temperature, const std::unique_ptr<ParticleContainer>& particle_container) {
    if (dimensions < 2 || dimensions > 3) {
        Logger::logger->error("Invalid number of dimensions: {}. Must be 2 or 3.", dimensions);
        throw std::runtime_error("Invalid number of dimensions");
    }

    for (auto it = particle_container->begin(); it != particle_container->end(); ++it) {
        it->setV(maxwellBoltzmannDistributedVelocity(std::sqrt(new_temperature / it->getM()), dimensions));
    }
}

void Thermostat::setParticleTemperature(double new_temperature, Particle& particle, size_t dimensions) {
    if (dimensions < 2 || dimensions > 3) {
        Logger::logger->error("Invalid number of dimensions: {}. Must be 2 or 3.", dimensions);
        throw std::runtime_error("Invalid number of dimensions");
    }

    particle.setV(particle.getV() + maxwellBoltzmannDistributedVelocity(std::sqrt(new_temperature / particle.getM()), dimensions));
}

double Thermostat::getTargetTemperature() const { return target_temperature; }

double Thermostat::getMaxTemperatureChange() const { return max_temperature_change; }

size_t Thermostat::getApplicationInterval() const { return application_interval; }

size_t Thermostat::getDimensions() const { return dimensions; }