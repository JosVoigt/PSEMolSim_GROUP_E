#include "CheckPointWriter.h"

#include <spdlog/fmt/bundled/core.h>

#include <fstream>

#include "io/xml_schemas/xsd_type_adaptors/InternalToXSDTypeAdapter.h"

const std::filesystem::path CheckPointWriter::writeFile(const SimulationParams& params, size_t iteration,
                                                        const std::vector<Particle>& particles) const {
    auto file_name = params.output_dir_path / fmt::format("MD_CHKPT_{:04d}.chkpt", iteration);

    MetaDataDataType meta_data{params.input_file_path.string(), params.input_file_hash, params.end_time, params.delta_t};

    CheckPointFileType::ParticleData_type xsd_particles{};

    xsd_particles.particle().reserve(particles.size());

    for (const Particle& particle : particles) {
        xsd_particles.particle().push_back(InternalToXSDTypeAdapter::convertToParticle(particle));
    }

    CheckPointFileType checkpointfile(meta_data, xsd_particles);

    std::ofstream file(file_name);
    CheckPoint(file, checkpointfile);
    file.close();

    return file_name;
}