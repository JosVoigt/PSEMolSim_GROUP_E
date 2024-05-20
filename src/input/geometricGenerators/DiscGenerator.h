#include "input/Initializer.h"

class DiscGenerator : public Initializer {
private:
  double radius;
  double mass;
  std::array<double, 3> center;

public:
  DiscGenerator(double radius, double mass, std::array<double, 3> &center);
  void readData(std::list<Particle> &list) override;
};
