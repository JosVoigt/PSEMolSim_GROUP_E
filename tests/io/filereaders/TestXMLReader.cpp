#include <gtest/gtest.h>

#include "data/FileLoader.h"
#include "io/input/chkpt/ChkptPointFileReader.h"
#include "io/input/xml/XMLFileReader.h"
#include "particles/spawners/sphere/SphereSpawner.h"
#include "utils/ArrayUtils.h"

#define EXPECT_NOT_CONTAINS_POS_NEAR(list, point, tol)                                                                                  \
    EXPECT_FALSE(std::find_if((list).begin(), (list).end(), [&](auto& x) { return ArrayUtils::L2Norm(x.getX() - (point)) < (tol); }) != \
                 (list).end());

TEST(XMLFileReader, CorrectParticleContainer) {
    XMLFileReader file_reader(true);

    auto [particles, params] = file_reader.readFile(FileLoader::get_input_file_path("test_collision.xml"));

    auto conf = *params;

    double err = 1e-13;
    EXPECT_NEAR(conf.end_time, 100, err);
    EXPECT_NEAR(conf.delta_t, 0.0014, err);

    EXPECT_EQ(particles.size(), 10 * 2 * 4 + 3 * 3 * 3 + 1);

    EXPECT_EQ(conf.container_type.index(), 0);

    for (int i = 0; i < 80; i++) {
        EXPECT_NEAR((particles)[i].getM(), 1, err);
    }
    for (int i = 80; i < 107; i++) {
        EXPECT_NEAR((particles)[i].getM(), 1.5, err);
        EXPECT_EQ(2, (particles)[i].getType());
    }
    EXPECT_EQ(10, (particles)[107].getType());
    EXPECT_NEAR((particles)[107].getM(), 100, err);

    EXPECT_NEAR((particles)[0].getX()[0], 0, err);
    EXPECT_NEAR((particles)[0].getX()[1], 0, err);
    EXPECT_NEAR((particles)[0].getX()[2], 0, err);

    EXPECT_NEAR((particles)[80].getX()[0], 100, err);
    EXPECT_NEAR((particles)[80].getX()[1], 20, err);
    EXPECT_NEAR((particles)[80].getX()[2], 20, err);

    EXPECT_NEAR((particles)[107].getX()[0], 20, err);
    EXPECT_NEAR((particles)[107].getX()[1], 100, err);
    EXPECT_NEAR((particles)[107].getX()[2], 100, err);
}

TEST(XMLFileReader, LoadCheckPoint) {
    XMLFileReader file_reader(true);
    ChkptPointFileReader chkpt_reader;

    auto [particles_xml, params_xml] = file_reader.readFile(FileLoader::get_input_file_path("CheckpointLoad.xml"));
    auto [particles_chkpt, params_chkpt] = chkpt_reader.readFile(FileLoader::get_input_file_path("ChktpExample.chkpt"));

    EXPECT_EQ(particles_xml.size(), particles_chkpt.size());

    for (size_t i = 0; i < particles_xml.size(); i++) {
        auto& particle_xml = particles_xml[i];
        auto& particle_chkpt = particles_chkpt[i];

        EXPECT_EQ(particle_xml, particle_chkpt);
    }
}

TEST(XMLFileReader, DoubleRecursiveSubSimulation) {
    XMLFileReader file_reader(true);

    auto [particles_xml, params_xml] = file_reader.readFile(FileLoader::get_input_file_path("RecursiveExample2.xml"));

    std::vector<Particle> equilibrated_particles;

    EXPECT_EQ(particles_xml.size(), 33 + 8 + 64);
}

TEST(XMLFileReader, DoubleRecursiveLoadCachedResults) {
    XMLFileReader file_reader(false);

    auto [particles_xml, params_xml] = file_reader.readFile(FileLoader::get_input_file_path("RecursiveExample2.xml"));

    std::vector<Particle> equilibrated_particles;

    EXPECT_EQ(particles_xml.size(), 33 + 8 + 64);
}
