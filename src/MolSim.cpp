#define NO_OUT_FILE

#include <gtest/gtest.h>

#include "container/ParticleContainer.h"
#include "input/FileReader.h"
#include "simulation/Simulation.h"
#include "utils/ArrayUtils.h"
#include "utils/Parser.h"
#include "logger/logger.h"
#include "input/CuboidGenerator.h"

int main(int argc, char* argv[]) {
    initLoggers();
    options opts = parse(argc, argv);

    if (opts.executeTests) {
        log_file (info, "Test flag was set, will proceed to execute tests");
        testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    } else {
        std::stringstream opt_string;

        opt_string << "Parsed options were:\n"
                   << "    execute tests: " << (opts.executeTests ? "true" : "false") << " (expected to be false)" "\n"
                   << "    delta_t: " << opts.delta_t << "\n"
                   << "    start: " << opts.start << "\n"
                   << "    end: " << opts.end << "\n"
                   << "    writeout frequency: " << opts.writeoutFrequency << "\n"
                   << "    file(s): " << ArrayUtils::to_string(opts.filepath) << "\n"
                   << "    outfile prefix: " << opts.outfile << "\n"
                   << "    writer method: " << opts.writer_->typeString() << "\n"
                   << "    output method: " << opts.force_->typeString() << "\n";

        std::stringstream expected_stream;

        expected_stream << "The expected behaivour would be: " << "\n"
                        << "    Generated files: " << (opts.end - opts.start) / opts.writeoutFrequency / opts.delta_t << "\n"
                        << "    Iterations: " << (opts.end / opts.delta_t) << "\n";

        log_file(info, opt_string.str());
        log_file(info, expected_stream.str());

        std::list<Particle> init = std::list<Particle>();

        for (const auto& file : opts.filepath) {
            FileReader fileReader(file.c_str());
            fileReader.readData(init);
        }

        for (auto& cuboid: opts.cuboids) {
            cuboid.readData(init);
        }

        std::stringstream particleCount;

        particleCount << "Particle count: " << init.size();
        log_file (debug, particleCount.str());

        if (init.size() < 2) {
            log_error(critical, "The simulation requires at least 2 particles! Include them via file or cuboid.");
            exit(1);
        }

        ParticleContainer container = ParticleContainer(init.size(), init);

        Simulation sim(container, opts.force_, opts.writer_, opts.delta_t,
                       opts.writeoutFrequency, opts.outfile);

        sim.run(opts.start, opts.end);
    }
}
