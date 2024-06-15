#include "container/Particle.h"
#include "force/LennardJonesForce.h"
#include <benchmark/benchmark.h>

static void testBench (benchmark::State& state) {
	LennardJonesForce ljf = LennardJonesForce(1,1);

	std::array<double, 3> zero = {0,0,0};
	Particle p = Particle(zero, zero, 0);
	Particle inc = Particle (zero, zero, 0);
	std::array<double,3> one = {1,0,0};

	for (int i = 0; i < 10000; i++) {	
		inc.addX(one);
		ljf.calculateForce(p, inc);
	} 
}

BENCHMARK(testBench);
