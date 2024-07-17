#include <utility>

#include "UpwardsForce.h"

UpwardsForce::UpwardsForce(std::array<double, 3> force, std::vector<int> target_particles, double end_time)
: force(force), target_particles(std::move(target_particles)), end_time(end_time) {}

void UpwardsForce::calculateForce(std::vector<Particle>& particles) const {
    for(auto& t : target_particles) {
        auto currentForce = particles[t].getF();
        for (size_t i = 0; i < 3; ++i) {
            currentForce[i] += force[i];
        }
        particles[t].setF(currentForce);
    }
}

UpwardsForce::operator std::string() const { return "UpwardsForce"; }


