#include "FileInputHandler.h"

#include <filesystem>
#include <iostream>

#include "io/logger/Logger.h"

std::tuple<std::vector<Particle>, std::optional<SimulationParams>> FileInputHandler::readFile(const std::filesystem::path& input_file_path,
                                                                                              bool fresh, bool allow_recursion) {
    if (!std::filesystem::exists(input_file_path)) {
        Logger::logger->error("Error: file '{}' does not exist.", input_file_path.string());
        throw FileReader::FileFormatException("File does not exist");
    }

    std::string file_extension = input_file_path.extension().string();

    if (get_supported_input_file_extensions().find(file_extension) == get_supported_input_file_extensions().end()) {
        Logger::logger->error("Error: file extension '{}' is not supported.", file_extension);
        throw FileReader::FileFormatException("File extension is not supported");
    }

    std::unique_ptr<FileReader> file_reader;

    if (file_extension == ".xml") {
        file_reader = std::make_unique<XMLFileReader>(fresh, allow_recursion);
    } else if (file_extension == ".chkpt") {
        file_reader = std::make_unique<ChkptPointFileReader>();
    } else {
        Logger::logger->error("Error: file extension '{}' is not supported.", file_extension);
        throw FileReader::FileFormatException("File extension is not supported");
    }

    try {
        auto [particles, config] = file_reader->readFile(input_file_path);
        Logger::logger->info("Loaded {} particles from file {}", particles.size(), input_file_path.string());
        return std::make_tuple(particles, config);
    } catch (const FileReader::FileFormatException& e) {
        Logger::logger->error("Error: file '{}' is not a valid {} file.", input_file_path.string(), file_extension);
        Logger::logger->error("FileFormatException:\n{}", std::string(e.what()));
        throw;
    }
}
