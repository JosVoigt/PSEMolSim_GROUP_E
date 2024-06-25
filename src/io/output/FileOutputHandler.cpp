#include "FileOutputHandler.h"

#include <filesystem>

#include "io/logger/Logger.h"
#include "io/output/chkpt/CheckPointWriter.h"
#include "io/output/vtu/VTUWriter.h"
#include "io/output/xyz/XYZWriter.h"

FileOutputHandler::FileOutputHandler(const OutputFormat output_format, const SimulationParams& params) : params(params) {
    switch (output_format) {
        case OutputFormat::VTU:
            file_writer = std::make_unique<VTUWriter>();
            break;
        case OutputFormat::XYZ:
            file_writer = std::make_unique<XYZWriter>();
            break;
        case OutputFormat::CHKPT:
            file_writer = std::make_unique<CheckPointWriter>();
            break;
        default:
            Logger::logger->error("Output format not implemented.");
            exit(1);
    }

    if (std::filesystem::exists(params.output_dir_path)) {
        auto supported = get_supported_output_formats();
        auto file_extension = std::find_if(supported.begin(), supported.end(), [&params, &output_format](const auto& pair) {
                                  return pair.second == output_format;
                              })->first;

        auto count = 0;
        for (const auto& entry : std::filesystem::directory_iterator(params.output_dir_path)) {
            if (entry.path().extension() == "." + file_extension) {
                std::filesystem::remove(entry.path());
                count++;
            }
        }
        if (count > 0) {
            Logger::logger->warn("Removed {} files with targetted file extension {} from target directory {}", count, file_extension,
                                 params.output_dir_path.string());
        }
    } else {
        Logger::logger->info("Creating output directory '{}'.", params.output_dir_path.string());
        std::filesystem::create_directories(params.output_dir_path.string());
    }
}

std::optional<const std::filesystem::path> FileOutputHandler::writeFile(size_t iteration, const std::vector<Particle>& particles) const {
    if (!file_writer) {
        Logger::logger->warn("No file writer set. Not writing file.");
        throw std::runtime_error("No file writer set. Not writing file.");
    }
    return file_writer->writeFile(params, iteration, particles);
}

std::optional<const std::filesystem::path> FileOutputHandler::writeFile(
    size_t iteration, const std::unique_ptr<ParticleContainer>& particle_container) const {
    return writeFile(iteration, particle_container->getParticles());
}
