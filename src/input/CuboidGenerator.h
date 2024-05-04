#include <array>

#include "Initializer.h"

class CuboidGenerator : public Initializer {
   private:
    int x;
    int y;
    int z;
    double h;
    double mass;
    std::array<double, 3> lowerLeftFrontCorner;
    std::array<double, 3> initialVelocity;

   public:
    CuboidGenerator(int x_, int y_, int z_, double distance, double mass_,
                    std::array<double, 3> lowerLeftFrontCorner_,
                    std::array<double, 3> initialVelocity_);
    ~CuboidGenerator() override = default;

    void readData(std::list<Particle>& list) override;
};
