#pragma once

#include <memory>
#include <string>

#include "io/output/FileWriter.h"
#include "io/output/OutputFormats.h"
#include "particles/containers/ParticleContainer.h"

/**
 * @brief Wrapper class to abstract the writing of output files
 *
 * Currently there are two supported output formats: VTU and XYZ. Additionally a 'NONE' format is available, which does not write any
 * output.
 */
class FileOutputHandler {
   private:
    /**
     * @brief SimulationParams object which provides the output directory path
     */
    const SimulationParams& params;

    /**
     * @brief Pointer to the FileWriter object to use
     */
    std::unique_ptr<FileWriter> file_writer;

   public:
    /**
     * @brief Construct a new FileOutputHandler object
     *
     * @param output_format The output format to use
     * @param params SimulationParams object which provides the output directory path
     */
    explicit FileOutputHandler(const OutputFormat output_format, const SimulationParams& params);

    /**
     * @brief Writes the given ParticleContainers particle data to a file
     *
     * @param iteration The current iteration number of the simulation
     * @param particle_container The ParticleContainer to write to the file
     *
     * @return The path to the written file
     */
    std::optional<const std::filesystem::path> writeFile(size_t iteration,
                                                         const std::unique_ptr<ParticleContainer>& particle_container) const;

    /**
     * @brief Writes the given ParticleContainers particle data to a file
     *
     * @param iteration The current iteration number of the simulation
     * @param particles A vector of particles to write to the file
     *
     * @return The path to the written file
     */
    std::optional<const std::filesystem::path> writeFile(size_t iteration, const std::vector<Particle>& particles) const;
};