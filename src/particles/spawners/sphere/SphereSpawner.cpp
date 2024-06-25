#include "SphereSpawner.h"

#include "particles/Particle.h"
#include "physics/thermostats/Thermostat.h"
#include "utils/ArrayUtils.h"

SphereSpawner::SphereSpawner(const std::array<double, 3>& center, const int sphere_radius, double grid_spacing, double mass,
                             const std::array<double, 3>& initial_velocity, int type, double epsilon, double sigma, bool third_dimension,
                             double initial_temperature)
    : center(center),
      sphere_radius(sphere_radius),
      grid_spacing(grid_spacing),
      mass(mass),
      type(type),
      espilon(epsilon),
      sigma(sigma),
      initial_velocity(initial_velocity),
      initial_temperature(initial_temperature),
      third_dimension(third_dimension) {}

int SphereSpawner::spawnParticles(std::vector<Particle>& particles) const {
    particles.reserve(particles.size() + getEstimatedNumberOfParticles());
    int num_particles_spawned = 0;
    for (int x = -sphere_radius; x <= sphere_radius; x++) {
        for (int y = -sphere_radius; y <= sphere_radius; y++) {
            for (int z = -sphere_radius; z <= sphere_radius; z++) {
                if (!third_dimension && z != 0) {
                    continue;
                }

                const auto displacement = std::array<double, 3>{x * grid_spacing, y * grid_spacing, z * grid_spacing};
                const double dist = ArrayUtils::L2Norm(displacement);

                if (dist <= sphere_radius * grid_spacing) {
                    const auto position = center + displacement;
                    Particle particle(position, initial_velocity, mass, type, espilon, sigma);
                    Thermostat::setParticleTemperature(initial_temperature, particle, third_dimension ? 3 : 2);

                    particles.push_back(std::move(particle));
                    num_particles_spawned++;
                }
            }
        }
    }
    return num_particles_spawned;
}

size_t SphereSpawner::getEstimatedNumberOfParticles() const { return static_cast<size_t>(4.0 / 3.0 * M_PI * std::pow(sphere_radius, 3)); }