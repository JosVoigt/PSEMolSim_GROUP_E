#pragma once

#include "io/output/FileWriter.h"
#include "io/xml_schemas/vtu_file/vtu_file_schema.h"
#include "particles/Particle.h"

/**
 * @brief Class to write particle data to a .vtu file
 */
class VTUWriter : public FileWriter {
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

   private:
    /**
     * @brief Creates a VTKFile_t object with the given number of particles.
     *
     * @param numParticles Number of particles to be plotted
     * @return VTUFile_t object with the given number of particles
     */
    [[nodiscard]] static VTKFile_t initializeOutput(int numParticles);

    /**
     * @brief Writes a given particle to the given VTKFile_t object.
     *
     * @param file VTUFile_t object to write to
     * @param p Particle to be written
     */
    static void plotParticle(VTKFile_t& file, const Particle& p);
};
