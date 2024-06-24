#include <benchmark/benchmark.h>
#include <iostream>

#include "container/Particle.h"
#include "force/LennardJonesForce.h"

#ifndef NO_LOG
int main () {
	std::cout << "Please disable logging to run the benchmarks. To do so call cmake with -DLOGGING=OFF" << std::endl;
	return 0;
}
#else
static void testBench(benchmark::State& state) {
  LennardJonesForce ljf = LennardJonesForce(1, 1);

  std::array<double, 3> zero = {0, 0, 0};
  Particle p = Particle(zero, zero, 0);
  Particle inc = Particle(zero, zero, 0);

  for (auto _ : state) {
    ljf.calculateForce(p, inc);
  }
}

BENCHMARK(testBench);
#endif
