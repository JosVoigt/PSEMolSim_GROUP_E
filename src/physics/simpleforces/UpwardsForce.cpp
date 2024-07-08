#include "UpwardsForce.h"

UpwardsForce::UpwardsForce(std::array<double, 3> force, std::vector<int>& target_particles, double end_time)
: force(force), target_particles(target_particles), end_time(end_time) {}

void UpwardsForce::calculateForce(std::vector<Particle>& particle_vector, double time) const {
    if(time < end_time) {
        for(auto& t : target_particles) {
            Particle p = particle_vector[t];
            std::array<double, 3> newForce{};
            for(int i = 0; i < 3; i++) {
                newForce[i] = p.getF()[i] + force[i];
            }
            p.setF(newForce);
        }
    }
}
