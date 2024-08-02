#include "bin_counter.h"

#include <cmath>
#include <fstream>

#include "simulation/Simulation.h"
#include "utils/ArrayUtils.h"
#include "io/logger/Logger.h"

void BinInterceptor::operator()(size_t iteration, Simulation &simulation) {
  //array temporary storage for bin values
  std::vector<std::vector<std::array<double, 3>>> value_storage;
  //loop over all particles
  for (auto p : *simulation.particle_container) {
    //calculate respective bin
    int bin = std::floor((p.getX()[0] - bin_start_x) / bin_size);

    //check if bin is in bounds
    if (bin < 0 || bin >= bin_count) {
      //out of bounds
      continue;
    } else {
      value_storage.at(value_storage.size() - 1).emplace_back(p.getV());
    }
  }
  data.emplace_back();
  for (auto bin : value_storage) {
    double density = bin.size() / bin_volume;

    //calculate avg velocity
    std::array<double, 3> velo_sum = {0, 0, 0};
    for (auto velo : bin) {
      velo_sum = velo_sum + velo;
    }
    velo_sum = (1 / bin.size()) * velo_sum;

    //push back into data
    data.at(data.size() - 1).push_back({density, velo_sum[1]});
  }
}

void BinInterceptor::onSimulationStart(Simulation &simulation) { return; }

void BinInterceptor::onSimulationEnd(size_t iteration, Simulation &simulation) {
  std::ofstream outputFileStream(outputFileName + ".csv");

  outputFileStream << "Bins:;" << bin_count << "\n"
                   << "Calculated bin volume:;" << bin_volume << "\n"
                   << "Total particle count:;"
                   << simulation.particle_container->size() << "\n";

  for (auto ita : data) {
    outputFileStream << "\n";

    for (auto bin : ita) {
      outputFileStream << bin.first << bin.second;
    }
  }

  outputFileStream.close();
}

BinInterceptor::operator std::string() const { return "Bin Interceptor"; }

void BinInterceptor::logSummary(int depth) const {
  Logger::logger->info("Bin Interceptor, Output path -> {}.csv",
                       outputFileName);
}
