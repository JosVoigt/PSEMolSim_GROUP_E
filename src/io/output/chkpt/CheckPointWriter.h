#pragma once

#include "io/output/FileWriter.h"
#include "io/xml_schemas/checkpoint/checkpoint_schema.h"
#include "particles/Particle.h"

/**
 * @brief Class to write particle data to a .chkpt file
 */
class CheckPointWriter : public FileWriter {
   public:
    /**
     * @brief Writes the data of the given ParticleContainer to a .vtu file
     *
     * @param params SimulationParams object which provides the output directory path
     * @param iteration The current iteration number
     * @param particles A vector of particles to write to the file
     *
     * @return The path to the written file
     */
    const std::filesystem::path writeFile(const SimulationParams& params, size_t iteration,
                                          const std::vector<Particle>& particles) const override;
};
