#pragma once

#include "../Initializer.h"

class DiscGenerator : public Initializer {
private:
  int radius;
  double mass;
  double distance;
  std::array<double, 3> velocity;
  std::array<double, 3> center;

public:
  DiscGenerator(int radius, double distance, double mass, std::array<double, 3> velocity,  std::array<double, 3> &center);

  ~DiscGenerator() override = default;

  void readData(std::list<Particle> &list, int dimensions) override;
};
