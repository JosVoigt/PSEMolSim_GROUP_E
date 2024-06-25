#pragma once

#include <chrono>

#include "simulation/interceptors/SimulationInterceptor.h"

class ParticleUpdateCounterInterceptor : public SimulationInterceptor {
   public:
    /**
     * @brief This function is sets the particle_updates to 0 and initializes
     * the start time of the simulation
     *
     * @param simulation The simulation object
     */
    void onSimulationStart(Simulation& simulation) override;

    /**
     * @brief This function is called on every nth iteration. It counts the
     * number of particle updates which have been performed.
     *
     * @param iteration The current iteration
     * @param simulation The simulation object
     */
    void operator()(size_t iteration, Simulation& simulation) override;

    /**
     * @brief This function is empty as the thermostat doesnt need to do anything
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
     * @brief Logs the summary of the particle update counter
     */
    void logSummary(int depth) const override;

    /**
     * @brief Get the particle updates per second
     *
     * @return double
     */
    [[nodiscard]] double getParticleUpdatesPerSecond() const;

   private:
    size_t particle_updates = 0;
    std::chrono::high_resolution_clock::time_point t_start;
    std::chrono::high_resolution_clock::time_point t_end;
    std::chrono::milliseconds::rep t_diff{};
    double particle_updates_per_second{};
};