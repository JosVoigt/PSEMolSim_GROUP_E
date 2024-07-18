#include "MaxwellBoltzmannDistribution.h"

std::array<double, 3> maxwellBoltzmannDistributedVelocity(double averageVelocity, size_t dimensions) {
    // we use a constant seed for repeatability.
    // random engine needs static lifetime otherwise it would be recreated for every call.
    static std::default_random_engine random_engine(42);

    // when adding independent normally distributed values to all velocity components
    // the velocity change is maxwell boltzmann distributed
    std::normal_distribution<double> normal_distribution{0, 1};
    std::array<double, 3> random_velocity{};
    for (size_t i = 0; i < dimensions; ++i) {
        random_velocity[i] = averageVelocity * normal_distribution(random_engine);
    }
    return random_velocity;
}
