#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

#include "particles/containers/linkedcells/LinkedCellsContainer.h"
#include "physics/pairwiseforces/PairwiseForceSource.h"
#include "physics/simpleforces/SimpleForceSource.h"
#include "simulation/interceptors/SimulationInterceptor.h"

/**
 * @brief Contains all parameters needed to run a simulation.
 *
 * This class is used to pass the parameters from the input file / CLI to the Simulation class.
 */
class SimulationParams {
   public:
    /**
     * @brief Struct to specify the type of the particle container as DirectSumType
     */
    struct DirectSumType {
        explicit operator std::string() const { return "DirectSum"; }
    };

    /**
     * @brief Struct to specify the type of the particle container as LinkedCellsType (needs domain_size and cutoff_radius)
     */
    struct LinkedCellsType {
        std::array<double, 3> domain_size;
        double cutoff_radius;
        std::array<LinkedCellsContainer::BoundaryCondition, 6> boundary_conditions;

        LinkedCellsType() = delete;
        LinkedCellsType(const std::array<double, 3>& domain_size, double cutoff_radius,
                        const std::array<LinkedCellsContainer::BoundaryCondition, 6>& boundary_conditions)
            : domain_size(domain_size), cutoff_radius(cutoff_radius), boundary_conditions(boundary_conditions) {}

        explicit operator std::string() const {
            return "LinkedCells ([" + std::to_string(domain_size[0]) + "x" + std::to_string(domain_size[1]) + "x" +
                   std::to_string(domain_size[2]) + "];" + std::to_string(cutoff_radius) + ";[" +
                   LinkedCellsContainer::boundaryConditionToString(boundary_conditions[0]) + "|" +
                   LinkedCellsContainer::boundaryConditionToString(boundary_conditions[1]) + "|" +
                   LinkedCellsContainer::boundaryConditionToString(boundary_conditions[2]) + "|" +
                   LinkedCellsContainer::boundaryConditionToString(boundary_conditions[3]) + "|" +
                   LinkedCellsContainer::boundaryConditionToString(boundary_conditions[4]) + "|" +
                   LinkedCellsContainer::boundaryConditionToString(boundary_conditions[5]) + "])";
        }
    };

    /**
     * @brief Path to the input file of the simulation
     */
    std::filesystem::path input_file_path;

    /**
     * @brief Hash of the input file of the simulation
     */
    std::size_t input_file_hash;

    /**
     * @brief Path to the directory in which to save the simulation output
     */
    std::filesystem::path output_dir_path;

    /**
     * @brief Time step of a single simulation iteration
     */
    double delta_t;

    /**
     * @brief End time of the simulation
     */
    double end_time;

    /**
     * @brief List of interceptors to be used in the simulation
     */
    std::vector<std::shared_ptr<SimulationInterceptor>> interceptors;

    /**
     * @brief Type of the particle container
     */
    std::variant<DirectSumType, LinkedCellsType> container_type;

    /**
     * @brief Simple Forces to be applied to the particles
     */
    std::vector<std::shared_ptr<SimpleForceSource>> simple_forces;

    /**
     * @brief Pairwise Forces to be applied to the particles
     */
    std::vector<std::shared_ptr<PairwiseForceSource>> pairwise_forces;

    /**
     * @brief Number of particles in the simulation
     */
    size_t num_particles;

    /**
     * @brief Whether to run the simulation in performance test mode
     */
    bool performance_test;

    /**
     * @brief Flag to indicate whether the simulation should be run from scratch, or whether cached data should be used
     */
    bool fresh;

    /**
     * @brief Construct a new SimulationParams object
     *
     * @param input_file_path Path to the input file of the simulation
     * @param delta_t Time step of a single simulation iteration
     * @param end_time End time of the simulation
     * @param container_type Type of the particle container
     * @param interceptors List of interceptors to be used in the simulation
     * @param simple_forces Simple Forces to be applied to the particles
     * @param pairwise_forces Forces to be applied to the particles
     * @param performance_test Whether to run the simulation in performance test mode
     * @param fresh Flag to indicate whether the simulation should be run from scratch, or whether cached data should be used
     * @param base_path Base path to the output directory. This is used to construct the output directory path if none is given
     * explicitly. Defaults to "./output/"
     */
    SimulationParams(const std::filesystem::path& input_file_path, double delta_t, double end_time,
                     const std::variant<DirectSumType, LinkedCellsType>& container_type,
                     const std::vector<std::shared_ptr<SimulationInterceptor>>& interceptors,
                     const std::vector<std::shared_ptr<SimpleForceSource>>& simple_forces,
                     const std::vector<std::shared_ptr<PairwiseForceSource>>& pairwise_forces, bool performance_test = false,
                     bool fresh = false, const std::filesystem::path& base_path = "./output");

    /**
     * @brief Prints a summary of the simulation parameters to the console
     * @param depth determines the indentation of the log message
     */
    void logSummary(int depth = 0) const;
};