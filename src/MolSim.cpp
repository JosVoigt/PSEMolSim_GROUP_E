#include "container/ParticleContainer.h"
#include "input/FileReader.h"
#include "simulation/Simulation.h"
#include "utils/Parser.h"

int main(int argc, char *argv[]) {
    options o = parse(argc, argv);

    std::list<Particle> init = list<Particle>();

    FileReader filereader;
    filereader.readFile(init, o.filepath.c_str());

    ParticleContainer container = ParticleContainer(init.size(), init);

    Simulation sim(container, o.force_.get(), o.writer_.get(), o.delta_t,
                   o.writeoutFrequency, o.outfile);

    sim.run(o.start, o.end);
}
