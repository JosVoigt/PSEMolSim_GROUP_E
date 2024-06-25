#pragma once
#include <chrono>

#include "physics/thermostats/Thermostat.h"
#include "simulation/interceptors/SimulationInterceptor.h"

class ThermostatInterceptor : public SimulationInterceptor {
   public:
    /**
     * @brief Construct a new Thermostat Interceptor object
     */
    explicit ThermostatInterceptor(Thermostat& thermostat);

    /**
     * @brief This function is empty as the thermostat doesnt need initialization
     *
     * @param simulation The simulation object
     */
    void onSimulationStart(Simulation& simulation) override;

    /**
     * @brief This function is called on every nth iteration. It scales the
     * temperature of the particles in accordance with the thermostat.
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
     * @brief Logs the summary of the thermostat
     */
    void logSummary(int depth) const override;

   private:
    /**
     * @brief The thermostat that is used to scale the temperature
     */
    Thermostat thermostat;
};