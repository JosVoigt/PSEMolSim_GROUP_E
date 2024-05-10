#include "Force.h"

class LennardJonesMolecule : public Force {
   private:
    double epsilon;
    double sigma;
    double epsilon_24;

   public:
    LennardJonesMolecule(double epsilon, double sigma);
    ~LennardJonesMolecule() override = default;

    std::array<double, 3> calculateForce(Particle& p1,
                                         Particle& p2) const override;
    std::string typeString() override;
};
