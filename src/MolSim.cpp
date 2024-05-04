#include "container/ParticleContainer.h"
#include "input/FileReader.h"
#include "simulation/Simulation.h"
#include "utils/Parser.h"

int main(int argc, char *argv[]) {
    options o = parse(argc, argv);

    std::list<Particle> init = list<Particle>();

    for (auto file : o.filepath) {
        FileReader filereader(file.c_str());
        filereader.readData(init);
    }

    ParticleContainer container = ParticleContainer(init.size(), init);

    Simulation sim(container, o.force_.get(), o.writer_.get(), o.delta_t,
                   o.writeoutFrequency, o.outfile);

    sim.run(o.start, o.end);
}
