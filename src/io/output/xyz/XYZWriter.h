#pragma once

#include <fstream>
#include <list>

#include "io/output/FileWriter.h"
#include "particles/Particle.h"
#include "particles/containers/ParticleContainer.h"

/**
 * @brief Class to write particle data to a .xyz file
 */
class XYZWriter : public FileWriter {
   public:
    /**
     * @brief Writes the data of the given ParticleContainer to a .xyz file
     *
     * @param params SimulationParams object which provides the output directory path
     * @param iteration The current iteration
     * @param particles A vector of particles to write to the file
     *
     * @return The path to the written file
     */
    const std::filesystem::path writeFile(const SimulationParams& params, size_t iteration,
                                          const std::vector<Particle>& particles) const override;
};
