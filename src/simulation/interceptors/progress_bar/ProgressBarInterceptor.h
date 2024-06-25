#pragma once
#include <chrono>

#include "particles/Particle.h"
#include "simulation/interceptors/SimulationInterceptor.h"

class ProgressBarInterceptor : public SimulationInterceptor {
   public:
    /**
     * @brief This function initalized the start time of the simulation
     * and the previous time point
     *
     * @param simulation The simulation object
     */
    void onSimulationStart(Simulation& simulation) override;

    /**
     * @brief This function is called on every nth iteration. It prints a progress
     * bar to the console and updates the previous time point.
     *
     * @param iteration The current iteration
     * @param simulation The simulation object
     */
    void operator()(size_t iteration, Simulation& simulation) override;

    /**
     * @brief This function is empty as the progress bar doesnt need to do anything
     * at the end of the simulation
     *
     * @param iteration The current iteration
     * @param simulation The simulation object
     */
    void onSimulationEnd(size_t iteration, Simulation& simulation) override;

    /**
     * @brief The string representation of this interceptor
     *
     * @return std::string
     *
     * This is used to write the final summary of the Interceptors to the
     * console.
     */
    explicit operator std::string() const override;

    /**
     * @brief Logs the summary of the progress bar
     */
    void logSummary(int depth) const override;

   private:
    size_t expected_iterations;
    std::chrono::high_resolution_clock::time_point t_start;
    std::chrono::high_resolution_clock::time_point t_end;
    std::chrono::high_resolution_clock::time_point t_prev;
};