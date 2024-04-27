
#include <iostream>
#include <list>

#include "FileReader.h"
#include "outputWriter/XYZWriter.h"
#include "utils/ArrayUtils.h"
#include "utils/Parser.h"

/**** forward declaration of the calculation functions ****/

/**
 * calculate the force for all particles
 */
void calculateF();

/**
 * calculate the position for all particles
 */
void calculateX();

/**
 * calculate the position for all particles
 */
void calculateV();

/**
 * plot the particles to a xyz-file
 */
void plotParticles(int iteration);

constexpr double start_time = 0;
double end_time = 1000;
double delta_t = 0.014;

// TODO: what data structure to pick?
std::list<Particle> particles;

int main(int argc, char* argv[]) {
    options result = parse(argc, argv);

    FileReader fileReader;
    fileReader.readFile(particles, result.filepath.c_str());

    end_time = result.end;
    delta_t = result.delta_t;

    std::cout << "Calculating until: " << end_time << std::endl
              << "Stepsize: " << delta_t << std::endl
              << "Iterations: " << end_time / delta_t << std::endl
              << "Particle count: " << particles.size() << std::endl;

    double current_time = start_time;

    int iteration = 0;

    // for this loop, we assume: current x, current f and current v are known
    while (current_time < end_time) {
        // calculate new x
        calculateX();
        // calculate new f
        calculateF();
        // calculate new v
        calculateV();

        iteration++;
        if (iteration % 10 == 0) {
            plotParticles(iteration);
        }
        // std::cout << "Iteration " << iteration << " finished." << std::endl;

        current_time += delta_t;
    }

    std::cout << "output written. Terminating..." << std::endl;
    return 0;
}

void calculateF() {
    std::list<Particle>::iterator iterator;
    iterator = particles.begin();

    // prepare the current particles for next iteration
    for (auto& p : particles) {
        p.nextIteration();
    }

    // iterate over the particle pairs
    bool reachedParticle = false;
    for (auto& p1 : particles) {
        for (auto& p2 : particles) {
            // check if particle combination has not been calculated
            if (!reachedParticle) {
                reachedParticle = (p1 == p2);
            } else {
                // calculate force
                std::array<double, 3> force_p1_to_p2 = p1.calculateForce(p2);
                // add to 1st particle
                p1.addF(force_p1_to_p2);

                // Usage of Newton's 3rd law
                for (int i = 0; i < 3; i++) {
                    force_p1_to_p2[i] *= -1;
                }

                // add inverse to second particle
                p2.addF(force_p1_to_p2);
            }
        }
        reachedParticle = false;
    }
}

/*
    Calculate the new x for every particle based on its NEW FORCE and old
   postition
*/
void calculateX() {
    for (auto& p : particles) {
        p.calculateX(delta_t);
    }
}

/*
    Calculate the new v for every particle based on NEW LOCATION and NEW FORCE
   and old velocity
*/
void calculateV() {
    for (auto& p : particles) {
        p.calculateV(delta_t);
    }
}

void plotParticles(int iteration) {
    std::string out_name("MD_vtk");

    outputWriter::XYZWriter writer;
    writer.plotParticles(particles, out_name, iteration);
}
