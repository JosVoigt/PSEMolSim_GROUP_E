#pragma once

#include <functional>
#include <memory>

#include "simulation/Simulation.h"

class SimulationInterceptor {
   public:
    /**
     * @brief Destroy the Simulation Interceptor object
     */
    virtual ~SimulationInterceptor() = default;

    /**
     * @brief Called before the simulation loop starts
     *
     * @param simulation The simulation object
     */
    virtual void onSimulationStart(Simulation& simulation) = 0;

    /**
     * @brief Called on every nth iteration. This function should perform the
     * desired action.
     *
     * @param iteration The current iteration
     *
     * @param simulation The simulation object
     */
    virtual void operator()(size_t iteration, Simulation& simulation) = 0;

    /**
     * @brief Called after the simulation loop ends
     *
     * @param iteration The current iteration
     *
     * @param simulation The simulation object
     */
    virtual void onSimulationEnd(size_t iteration, Simulation& simulation) = 0;

    /**
     * @brief This function is called by the simulation loop on every iteration.
     * Whenever the iteration is a multiple of every_nth_iteration, the
     * operator() function is called.
     *
     * @param iteration The current iteration
     * @param simulation The simulation object
     * @see operator()
     */
    void notify(size_t iteration, Simulation& simulation) {
        if (iteration % every_nth_iteration == 0) {
            (*this)(iteration, simulation);
        }
    }

    /**
     * @brief The string representation of this interceptor
     *
     * @return std::string
     *
     * This is used to write the final summary of the Interceptors to the
     * console.
     */
    virtual explicit operator std::string() const = 0;

    /**
     * @brief Summary of the interceptor
     *
     * @param depth
     */
    virtual void logSummary(int depth) const = 0;

   protected:
    size_t every_nth_iteration = 1;
};
