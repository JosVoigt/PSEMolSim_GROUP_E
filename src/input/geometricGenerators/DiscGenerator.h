#include "input/Initializer.h"

class DiscGenerator : public Initializer {
private:
  int radius;
  double mass;
  double distance;
  double velocity;
  std::array<double, 3> center;

public:
  DiscGenerator(int radius, double distance, double mass, double velocity, std::array<double, 3> &center);
  void readData(std::list<Particle> &list) override;
};
