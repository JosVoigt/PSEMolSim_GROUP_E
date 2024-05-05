#pragma once

#include <memory>
#include <string>

#include "container/ParticleContainer.h"
#include "force/Force.h"
#include "outputWriter/Writer.h"

class Simulation {
   private:
    ParticleContainer container;
    Force* method;
    Writer* out;

    double dt;
    double dt_sq;
    int outputFrequency;

    std::string filename;

   public:
    Simulation(ParticleContainer& container_, Force* method_, Writer* writer_,
               double dt_, int outputFrequency, std::string filename_);

    void run(double start, double end);
};