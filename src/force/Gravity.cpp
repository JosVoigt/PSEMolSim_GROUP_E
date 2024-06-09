#include "Gravity.h"
#include <array>
#include "container/Particle.h"
#include "utils/ArrayUtils.h"

Gravity::Gravity() {
	//gravity constant in munich, from: https://www.ptb.de/cms/fileadmin/internet/fachabteilungen/abteilung_1/1.1_masse/1.15/tabelle.pdf
	//(physikalisch-technische Bundesanstalt, Bundesministerium Wirtschaft & Klima)
	gravConstant = 9.807232;
}
Gravity::Gravity(double gravConstant_) : gravConstant(gravConstant_) {};

std::array<double, 3> Gravity::calculateForce (Particle& p) {
	//force is calculated along negative Y
	std::array<double, 3> gravVector = {0, -gravConstant, 0};
	return p.getM()*gravVector;
}
