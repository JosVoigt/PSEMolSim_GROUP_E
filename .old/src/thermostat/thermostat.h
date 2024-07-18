#include <vector>

#include "container/Particle.h"
class Thermostat {
 private:
  double maxChangeRate;
  double k_B;
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
		 * /param k_B
		 * Boltzmanns constant
		 */
  Thermostat(double maxChangeRate_, int dimensions_, double k_B);

  /**
		 * /brief
		 * Constructor
		 * Changerate defaults to INFINITY, so change is instant
		 * /param dimensions
		 * The amount of dimensions the simulation is run in
		 * /param k_B
		 * Boltzmanns constant
		 */
  Thermostat(int dimensions, double k_B);

  /**
		 * /brief
		 * Slowly scales the temperature over many time steps towards the target temperature.
		 * Will not exceed the scaling rate provided in the constuctor.
		 * /param particles
		 * The array of particles that should be scaled
		 * /param targetTemperature
		 * The targeted temperature
		 */
  void adaptTemperature(std::vector<Particle>& particles,
                        double targetTemperature);
};
