#include <gtest/gtest.h>
#include <spdlog/cfg/env.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <chrono>
#include <ctime>

#include "container/ParticleContainer.h"
#include "input/CuboidGenerator.h"
#include "input/FileReader.h"
#include "simulation/Simulation.h"
#include "utils/ArrayUtils.h"
#include "utils/Parser.h"

int main(int argc, char* argv[]) {
    auto console_logger = spdlog::stderr_color_mt("console");
    try {
        std::stringstream ss;
        std::string time_format = "%Y-%m-%d_%H-%M-%S";

        auto start = std::chrono::system_clock::now();
        time_t start_time = std::chrono::system_clock::to_time_t(start);

        ss << "logs/MolSim_"
           << std::put_time(std::localtime(&start_time), time_format.c_str())
           << ".log";

        auto file_logger = spdlog::basic_logger_mt("file", ss.str());
    } catch (const spdlog::spdlog_ex& ex) {
        spdlog::get("console")->critical(
            "File logger could not be initalizied: {}", ex.what());
        exit(1);
    }
    spdlog::flush_every(std::chrono::seconds(5));

    spdlog::cfg::load_env_levels();
    parser::options opts = parser::parse(argc, argv);

    if (opts.executeTests) {
        spdlog::get("file")->info(
            "Test flag was set, will proceed to execute tests");
        testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    } else {
        std::stringstream opt_string;

        // prevents unwanted formatting
        // clang-format off
        opt_string << "Parsed options were:\n"
                   << "    execute tests: " << (opts.executeTests ? "true" : "false") << " (expected to be false)\n"
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

        // clang-format on

        spdlog::get("file")->info(opt_string.str());
        spdlog::get("file")->info(expected_stream.str());

        std::list<Particle> init = std::list<Particle>();

        for (const auto& file : opts.filepath) {
            FileReader fileReader(file.c_str());
            fileReader.readData(init);
        }

        for (auto& cuboid : opts.cuboids) {
            cuboid.readData(init);
        }

        std::stringstream particleCount;

        particleCount << "Particle count: " << init.size();
        spdlog::get("file")->debug(particleCount.str());

        if (init.size() < 2) {
            spdlog::get("console")->critical(
                "The simulation requires at least 2 particles! "
                "Include them via file or cuboid.");
            exit(1);
        }

        ParticleContainer container = ParticleContainer(init.size(), init);

        Simulation sim(container, opts.force_, opts.writer_, opts.delta_t,
                       opts.writeoutFrequency, opts.outfile);

        sim.run(opts.start, opts.end);
    }
}
