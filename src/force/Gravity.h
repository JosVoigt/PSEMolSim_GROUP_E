#include <array>
#include "GeneralForce.h"

class Gravity : public GeneralForce {
	private:
		double gravConstant;
	public:
		Gravity(double gravConstant_);
		Gravity(); // defaults to 9.81 for earth

		std::array<double, 3> calculateForce (Particle& gravityAffected) override;
};
