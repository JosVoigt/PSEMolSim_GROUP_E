#pragma once

#include <optional>
#include <set>

#include "io/input/InputFormats.h"
#include "io/input/chkpt/ChkptPointFileReader.h"
#include "io/input/xml/XMLFileReader.h"
#include "simulation/SimulationParams.h"

/**
 * @brief Wrapper class to abstract the reading of input files
 *
 * This class abstracts the reading and writing of files, so that the Simulation class does not have to know about the concrete
 * implementations. Automatically determines correct file format using the file extension.
 */
class FileInputHandler {
   public:
    /**
     * @brief Reads the input file and stores the particles in the given ParticleContainer. Other simulation parameters are returned as
     * SimulationParams object.
     *
     * @param input_file_path The path to the input file
     * @param fresh Whether to start a fresh simulation or reuse cached data
     * @param allow_recursion Whether to allow subsimulations to be started
     * @return std::tuple<std::vector<Particle>, std::optional<SimulationParams>> Tuple containing the particles and the parameters of the
     * file
     *
     * Reads the input file, generates particles and returns them in a vector.
     * Parameters are stored in a `SimulationParams` object and returned.
     * For more information about the output file formats, see \ref InputFileFormats "Input File Formats"
     */
    static std::tuple<std::vector<Particle>, std::optional<SimulationParams>> readFile(const std::filesystem::path& input_file_path,
                                                                                       bool fresh = false, bool allow_recursion = true);
};
