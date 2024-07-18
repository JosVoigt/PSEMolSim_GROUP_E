#pragma once

#include <filesystem>

#include "io/input/FileReader.h"
#include "io/xml_schemas/checkpoint/checkpoint_schema.h"
#include "simulation/SimulationParams.h"
/**
 * @brief Class to read particle and simulation data from a '.xml' file
 */
class ChkptPointFileReader : public FileReader {
   public:
    /**
     * @brief Reads particle data from a '.xml' file and returns a vector of particles
     *
     * @param filepath Path to the file to read
     */
    [[nodiscard]] std::tuple<std::vector<Particle>, std::optional<SimulationParams>> readFile(
        const std::filesystem::path& filepath) const override;

    /**
     * @brief Calculates the hash for the given input file
     *
     * @param filepath Path to the input file to calculate the hash for
     *
     * Each CheckPoint saves the hash of the original it was created from. This function calculates the hash of the
     * given file.
     */
    [[nodiscard]] static size_t calculateHash(const std::filesystem::path& filepath);

    /**
     * @brief Checks if the given file contains a valid hash generated from the original input file
     *
     * @param filepath Path of the checkpoint file to check
     *
     * Each CheckPoint saves the hash of the original it was created from. This function checks if the original file
     * still hashes to the same value. If not, the CheckPoint is invalid because it refers to a input file which has
     * been changed since the CheckPoint was created.
     */
    [[nodiscard]] static bool detectSourceFileChanges(const std::string& filepath);
};