#include <gtest/gtest.h>

#include <list>

#include "container/Particle.h"
#include "container/ParticleContainerBasic.h"
#include "simulation/StoermerVerlet.h"
#include "force/LennardJonesForce.h"

#ifndef TOLERANCE
#define TOLERANCE 1e-7;
#endif

class StoermerVerletTest : public testing::Test {
   protected:
    StoermerVerletTest() {
        std::list<Particle> init = std::list<Particle>();
        init.emplace_back(xp1, v, mass, 0);
        init.emplace_back(xp2, v, mass, 0);
        init.emplace_back(xp3, v, mass, 0);
        init.emplace_back(xp4, v, mass, 0);
        init.emplace_back(xp5, v, mass, 0);
        init.emplace_back(xp6, v, mass, 0);

        // particle container
        ParticleContainer pc = ParticleContainer(init.size(), init);
    }
    // initial particle positions
    std::array<double, 3> xp1 = {0, 0, 0};
    std::array<double, 3> xp2 = {1, 0, 0};
    std::array<double, 3> xp3 = {2, 0, 0};
    std::array<double, 3> xp4 = {3, 0, 0};
    std::array<double, 3> xp5 = {4, 0, 0};
    std::array<double, 3> xp6 = {5, 0, 0};

    // initial velocity
    std::array<double, 3> v = {0, 0, 0};

    // particle mass
    double mass = 1;
    std::shared_ptr<ParticleContainerInterface> pc = std::shared_ptr<ParticleContainerInterface>(new ParticleContainer);

    double tolerance = TOLERANCE;
};

TEST_F(StoermerVerletTest, testCalcX) {
    std::array<double, 3> force = {0, 0, 0.5};
    std::array<double, 3> velocity = {0, 1, 0};

    for (Particle& p : StoermerVerletTest::pc->preprocessParticles()) {
        p.addF(force);
        p.addV(velocity);
    }

    for (int i = 0; i < 5; i++) {
        calculateX(pc, 1, 1);

        for (int j = 0; j < pc->size(); j++) {
            EXPECT_NEAR((double)j, (*(pc->begin() + j)).getX()[0], tolerance);
            EXPECT_NEAR((double)i, (*(pc->begin() + j)).getX()[1], tolerance);
            EXPECT_NEAR((double)i, (*(pc->begin() + j)).getX()[2], tolerance);
        }
    }
}

TEST_F(StoermerVerletTest, testCalcV) {

    std::array<double, 3> force = {0, 0, 0.5};
    std::array<double, 3> velocity = {0, 1, 0};

    for (Particle& p : StoermerVerletTest::pc->preprocessParticles()) {
        p.addF(force);
        p.addV(velocity);
    }

    for (int i = 0; i < 5; i++) {
        calculateV(pc, 1);

        for (int j = 0; j < pc->size(); j++) {
            EXPECT_NEAR((double)j, (*(pc->begin() + j)).getV()[0], tolerance);
            EXPECT_NEAR((double)i, (*(pc->begin() + j)).getV()[1], tolerance);
            EXPECT_NEAR((double)i, (*(pc->begin() + j)).getV()[2], tolerance);
        }
    }
}

TEST_F(StoermerVerletTest, testCalcF) {

    std::array<double, 3> force = {0, 0, 0.5};
    std::array<double, 3> velocity = {0, 1, 0};

    std::shared_ptr<Force> method = std::shared_ptr<Force>(new LennardJonesForce(1,1));

    for (Particle& p : StoermerVerletTest::pc->preprocessParticles()) {
        p.addF(force);
        p.addV(velocity);
    }

    for (int i = 0; i < 5; i++) {
        calculateF(pc, method);

        for (int j = 0; j < pc->size(); j++) {
            EXPECT_NEAR((double)j, (*(pc->begin() + j)).getV()[0], tolerance);
            EXPECT_NEAR((double)i, (*(pc->begin() + j)).getV()[1], tolerance);
            EXPECT_NEAR((double)i, (*(pc->begin() + j)).getV()[2], tolerance);
        }
    }
}
