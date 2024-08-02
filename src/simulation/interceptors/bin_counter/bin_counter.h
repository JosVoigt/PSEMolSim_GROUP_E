#include <simulation/interceptors/SimulationInterceptor.h>

#include "simulation/Simulation.h"

class BinInterceptor : public SimulationInterceptor {
 private:
  int bin_count;
  double bin_size;
  double bin_volume;

  double bin_start_x;
  double bin_end_x;

  std::string outputFileName;

  /*
   * OUTER : iterations
   * INNER : bins
   * FIRST : density
   * SECOND : average velocity
   */
  std::vector<std::vector<std::pair<double,double>>> data;

 public:
  /**
		 * \brief
		 * Constructor for the bin interceptor
		 * \param bin_count_
		 *	The amount the given section should be subdivided by.
		 * \param bin_start_x_
		 *  The x value of the left side of the first bin.
		 *  Behaivour is undefined for end >= start
		 * \param bin_end_x_
		 *  The x value of the right side of the last bin
		 *  Behaivour is undefined for end >= start
		 *  \param outputFileName_
		 *  Name of the file the values will be written to
		 *  \param frequency
		 *  The calculation frequency of the simulation
		 *  \param ySize
		 *  The simulation size in y direction, will be used to calculate the volume and density of particles
		 *  \param zSize
		 *  The simulation size in z direction, will be used to calculate the volume and density of particles
		 */
  BinInterceptor(int binCount_, double binStartX_, double binEndX_,
                 std::string outputFileName_, int frequency, double ySize,
                 double zSize);

  /**
		 * \brief
		 * The data collection for the bins
		 * \param iteration
		 *  The current iteration
		 *  \param simulation
		 *  The simulation object that contains the necessary data
		 */
  void operator()(size_t iteration, Simulation& simulation) override;

  /**
		 * \brief
		 * Does nothing, exists to complete the Interceptor interface
		 */
  void onSimulationStart(Simulation& simulation) override;

  /**
		 * \brief
		 * Will start generating the output as CSV
		 * After this function is called the simulation is assumed to have ended.
		 *
		 * Do not call this or any other function on this object as they may not work as intended after
		 * this function has been called.
		 *
		 * \param iteration
		 * The last iteration the simulation is on
		 * \param simulation
		 * The simulation object containing all necessary information
		 */
  void onSimulationEnd(size_t iteration, Simulation& simulation) override;

  /**
		 * \brief
		 * tostring
		 */
  explicit operator std::string() const override;

  /**
		 * \brief
		 * Prints output path to console
		 * \param depth
		 * ignored
		 */
  void logSummary(int depth) const override;
};
