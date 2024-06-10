#include <vector>
#include "container/Particle.h"
class Thermostat {
	private:
		double maxChangeRate;
		int dimensions;
	public:
		/**
		 * /brief
		 *	Constructor
		 * /param maxChangeRate_
		 * Maximum change rate for the temperature
		 * Please provide it as as percentage of the current temp (e.g a max of 20% up or down would be mCR = 0.2)
		 * /param dimensions_
		 * The dimensions the simulation is run in
		 */
		Thermostat (double maxChangeRate_, int dimensions_);

		/**
		 * /brief
		 * Constructor
		 * Changerate defaults to INFINITY, so change is instant
		 * /param dimensions
		 * The amount of dimensions the simulation is run in
		 */
		Thermostat (int dimensions);

		/**
		 * /brief
		 * Slowly scales the temperature over many time steps towards the target temperature.
		 * Will not exceed the scaling rate provided in the constuctor.
		 * /param particles
		 * The array of particles that should be scaled
		 * /param targetTemperature
		 * The targeted temperature
		 */
		void adaptTemperature (std::vector<Particle>& particles, double targetTemperature);
};
