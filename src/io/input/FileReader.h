#pragma once

#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "particles/Particle.h"
#include "simulation/SimulationParams.h"

/**
 * @brief Abstract base class for all custom file readers
 */
class FileReader {
   public:
    /**
     * @brief virtual destructor for correct cleanup of derived classes
     */
    virtual ~FileReader() = default;

    /**
     * @brief Reads the file with the given path and returns a vector of particles
     * @param filepath Path to the file to be read
     */
    [[nodiscard]] virtual std::tuple<std::vector<Particle>, std::optional<SimulationParams>> readFile(
        const std::filesystem::path& filepath) const = 0;

    /**
     * @brief Exception to be thrown when the file format is invalid
     */
    class FileFormatException : public std::exception {
       public:
        explicit FileFormatException(std::string message) : message_(std::move(message)) {}

        [[nodiscard]] const char* what() const noexcept override { return message_.c_str(); }

       private:
        std::string message_;
    };
};
