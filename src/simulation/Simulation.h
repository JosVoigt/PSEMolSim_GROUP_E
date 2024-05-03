#pragma once

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

    std::string filename;

   public:
    Simulation(ParticleContainer& container_, Force* method_, Writer* writer,
               double dt_, std::string filename_);

    void run(double start, double end);
};