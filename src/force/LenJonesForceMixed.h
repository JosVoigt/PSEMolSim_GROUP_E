
#include <map>
#include <utility>
#include <vector>

#include "container/Particle.h"
#include "force/Force.h"

class LennardJonesForceMixed : public PairwiseForce {
 private:
  std::map<int, std::pair<double, double>> force_constants;

  static int inline z_curve_interleave(int a, int b) ;
  static std::pair<double, double> calculateMixingRule(double sig_a, double eps_a,
                                                double sig_b, double eps_b);

 public:
  LennardJonesForceMixed(std::vector<double> epsilon, std::vector<double> sigma,
                         std::vector<int> type);
  std::array<double, 3> calculateForce(Particle& p1,
                                       Particle& p2) const override;

    std::string typeString() override;
};
