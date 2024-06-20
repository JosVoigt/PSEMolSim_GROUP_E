#include <gtest/gtest.h>
#include <vector>

#include "container/Particle.h"
#include "thermostat/thermostat.h"

double error = 10e-7;


class T_Thermostat : public testing::Test {};


TEST_F (T_Thermostat, holdingTemp) {
	Particle p1, p2 ,p3;
	
	std::array<double, 3> x = {0,0,0};
	std::array<double, 3> v1 = {1,1,1};
	std::array<double, 3> v2 = {1,0,2};
	std::array<double, 3> v3 = {3,0,0};

	p1 = Particle(x,v1,2,0);
	p2 = Particle(x,v2,2,0);
	p3 = Particle(x,v3,2,0);

	Thermostat thermo = Thermostat(3,1.5);

	std::vector<Particle> p = {p1,p2,p3};

	thermo.adaptTemperature(p, 3);

	EXPECT_NEAR (p1.getV()[0], v1[0], error);
	EXPECT_NEAR (p1.getV()[1], v1[1], error);
	EXPECT_NEAR (p1.getV()[2], v1[2], error);
	EXPECT_NEAR (p2.getV()[0], v2[0], error);
	EXPECT_NEAR (p2.getV()[1], v2[1], error);
	EXPECT_NEAR (p2.getV()[2], v2[2], error);
	EXPECT_NEAR (p3.getV()[0], v3[0], error);
	EXPECT_NEAR (p3.getV()[1], v3[1], error);
	EXPECT_NEAR (p3.getV()[2], v3[2], error);
}
