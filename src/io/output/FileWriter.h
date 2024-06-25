#pragma once

#include <memory>
#include <optional>

#include "particles/Particle.h"
#include "simulation/SimulationParams.h"

/**
 * @brief Abstract base class for all file writers
 */
class FileWriter {
   public:
    /**
     * @brief virtual destructor for correct cleanup of derived classes
     */
    virtual ~FileWriter() = default;

    /**
     * @brief Writes the file to the given path, uses the given ParticleContainer and the current iteration
     *
     * @param params SimulationParams object which provides the output directory path
     * @param iteration The current iteration number
     * @param particles A vector of particles to write to the file
     *
     * @return The path to the written file
     */
    [[nodiscard]] virtual const std::filesystem::path writeFile(const SimulationParams& params, size_t iteration,
                                                                const std::vector<Particle>& particles) const = 0;
};