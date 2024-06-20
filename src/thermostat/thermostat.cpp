#include "thermostat.h"

#include <cstdlib>
#include <iostream>
#include <limits>
#include <vector>

#include "container/Particle.h"
#include "utils/ArrayUtils.h"

Thermostat::Thermostat(double maxChangeRate_, int dimensions_, double k_B_)
    : maxChangeRate(maxChangeRate_), dimensions(dimensions_), k_B(k_B_){};

Thermostat::Thermostat(int dimensions_, double k_B_)
    : maxChangeRate(std::numeric_limits<double>::infinity()),
      dimensions(dimensions_),
      k_B(k_B_){};

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
      kinEnergySum / (particleCount * dimensions * k_B);

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
