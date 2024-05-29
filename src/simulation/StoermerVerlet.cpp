#include "StoermerVerlet.h"

void calculateX(ParticleContainer &container, const double dt,
                const double dt_sq) {
    for (Particle &p : container) {
        std::array<double, 3> res =
            (dt * p.getV()) + (dt_sq / (2 * p.getM())) * p.getF();
        p.addX(res);
    }
}

void calculateV(ParticleContainer &container, double dt) {
    for (Particle &p : container) {
        std::array<double, 3> res =
            (dt / (2 * p.getM())) * (p.getF() + p.getOldF());
        p.addV(res);
    }
}

void calculateF(ParticleContainer &container,
                const std::shared_ptr<Force> method) {
    for (Particle &p : container) {
        p.nextIteration();
    }

    for (auto iterator = container.begin(); iterator != container.end();
         iterator++) {
        for (auto inner = iterator + 1; inner != container.end(); inner++) {
            std::array<double, 3> f = method->calculateForce(*iterator, *inner);
            iterator->addF(f);
            std::array<double, 3> invF = -1 * f;
            inner->addF(invF);
        }
    }
}
