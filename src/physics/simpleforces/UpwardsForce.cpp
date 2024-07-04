#include "UpwardsForce.h"

std::array<double, 3UL> UpwardsForce::calculateForce(Particle& p) const {
    return {0.0, 0.0, 0.8};
}
