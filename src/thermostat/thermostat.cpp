#include "thermostat.h"

#include <cstdlib>
#include <limits>
#include <vector>

#include "container/Particle.h"
#include "utils/ArrayUtils.h"

double const boltzmannConstant =
    1.380649 * 1e-23;  // https://de.wikipedia.org/wiki/Boltzmann-Konstante

Thermostat::Thermostat(double maxChangeRate_, int dimensions_)
    : maxChangeRate(maxChangeRate_), dimensions(dimensions_){};

Thermostat::Thermostat(int dimensions_)
    : maxChangeRate(std::numeric_limits<double>::infinity()),
      dimensions(dimensions_){};

void Thermostat::adaptTemperature(std::vector<Particle>& particles,
                                  double targetTemperature) {
  // calculate kinetic energy
  double kinEnergySum = 0;
  for (Particle& p : particles) {
    double particleVelocity = ArrayUtils::L2Norm(p.getV());
    kinEnergySum += (p.getM() * particleVelocity);
  }

  // calculate current temperature
  int particleCount = particles.size();

  double temperature =
      (2 * kinEnergySum) / (particleCount * dimensions * boltzmannConstant);

  double temperatureGradient = std::sqrt(targetTemperature / temperature);
  double scalingFactor;

  if (std::abs(temperature - (temperature * temperatureGradient)) >
      maxChangeRate) {
    scalingFactor = std::sqrt((temperature + maxChangeRate) / temperature);
  } else {
    scalingFactor = temperatureGradient;
  }

  for (Particle& p : particles) {
    p.scaleV(scalingFactor);
  }
}
