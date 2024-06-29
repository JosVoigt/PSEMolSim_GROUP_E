#include "XYZWriter.h"

#include <spdlog/fmt/bundled/core.h>

#include <iomanip>

const std::filesystem::path XYZWriter::writeFile(const SimulationParams& params, size_t iteration,
                                                 const std::vector<Particle>& particles) const {
    auto file_name = params.output_dir_path / fmt::format("MD_XYZ_{:04d}.xyz", iteration);

    std::ofstream file;

    file.open(file_name);
    file << particles.size() << std::endl;
    file << "Generated by MolSim. See http://openbabel.org/wiki/XYZ_(format) for file format doku." << std::endl;

    for (auto& p : particles) {
        std::array<double, 3> x = p.getX();
        file << "Ar ";
        file.setf(std::ios_base::showpoint);

        for (auto& xi : x) {
            file << xi << " ";
        }

        file << std::endl;
    }

    file.close();

    return file_name;
}