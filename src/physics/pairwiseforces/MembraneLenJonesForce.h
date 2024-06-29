#include "LennardJonesForce.h"

class MembraneLenJonesForce : public LennardJonesForce {
 private:
  double bondDistance;
  //bond strength
  double k;

 public:
  MembraneLenJonesForce(double bondDistance_, double bondStrength);
  std::array<double, 3> calculateForce(Particle& p1,
                                       Particle& p2) const override;
  explicit operator std::string() const override;
};
