#include <gtest/gtest.h>

#include "container/ParticleContainer.h"
#include "input/FileReader.h"
#include "simulation/Simulation.h"
#include "utils/Parser.h"

int main(int argc, char* argv[]) {
    options opts = parse(argc, argv);

    if (opts.executeTests) {
        testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    } else {
        std::list<Particle> init = std::list<Particle>();

        for (const auto& file : opts.filepath) {
            FileReader fileReader(file.c_str());
            fileReader.readData(init);
        }

        ParticleContainer container = ParticleContainer(init.size(), init);

        Simulation sim(container, opts.force_, opts.writer_, opts.delta_t,
                       opts.writeoutFrequency, opts.outfile);

        sim.run(opts.start, opts.end);
    }
}
