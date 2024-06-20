#include <unordered_map>
#include <utility>
#include <vector>

#include "container/Particle.h"
#include "force/Force.h"

class LennardJonesForceMixed : public PairwiseForce {
 private:
	 std::vector<double> epsilon;
	 std::vector<double> sigma;
	 std::vector<int> type;

  static int inline z_curve_interleave(int a, int b) ;
  static std::pair<double, double> calculateMixingRule(double sig_a, double eps_a,
                                                double sig_b, double eps_b);

 public:
  /**
   * \brief
   * Initializer for the force calculation.
   * The constructor will precalc the values for all possible combinations.
   *
   * It will use the fact that Lorenz-Berthelot mixing rule yields the same result for particle combinations and their mirror composition
   *
   * \param epsilon
   * The epsilon values for the different particle types
   * \param sigma
   * The sigma values for the different particle types
   * \param type
   * The type of the particles. The length of all lists has to match
   */
  LennardJonesForceMixed(std::vector<double> epsilon, std::vector<double> sigma,
                         std::vector<int> type);
  /**
   * \brief
   *  Calculates the force between two particles, taking their types into consideration
   *  Force will point from p1 to p2
   * \param p1
   * The first particle
   * \param p2
   * The second particle
   * \return
   * The force attracting/repelling p1 to/from p2
   */
  std::array<double, 3> calculateForce(Particle& p1,
                                       Particle& p2) const override;

    std::string typeString() override;
};
