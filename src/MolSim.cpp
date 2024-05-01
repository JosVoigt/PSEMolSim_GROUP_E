
#include <algorithm>
#include <iostream>
#include <vector>

#include "FileReader.h"
#include "Parser.h"
#include "Particle.h"
#include "Planet.h"
#include "outputWriter/XYZWriter.h"
#include "utils/ArrayUtils.h"

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
void plotParticles(int iteration, outputType type);

constexpr double start_time = 0;
double end_time;
double delta_t;

std::vector<Particle*> particles;

int main(int argc, char* argv[]) {
    options result = parse(argc, argv);

    FileReader fileReader;
    std::vector<Particle> partReadin;
    fileReader.readFile(partReadin, result.filepath.c_str());

    particles.reserve(partReadin.size());

    writer* w;

    switch (result.type) {
        case planet:
            for (auto& p : partReadin) {
                auto pl = Planet(p);
                particles.push_back(&pl);
            }
            break;
    }

    if (result.output_method = vtk) {
        auto vtk = outputWriter::VTKWriter();
        w = (writer*)&vtk;
    } else if (result.output_method = xyz) {
        auto xyz = outputWriter::XYZWriter();
        w = (writer*)&xyz;
    }

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
            plotParticles(iteration, result.output_method);
        }
        // std::cout << "Iteration " << iteration << " finished." << std::endl;

        current_time += delta_t;
    }

    std::cout << "output written. Terminating..." << std::endl;
    return 0;
}

/**
 * \brief
 *  Calculates the forces between every particle in the particle list.
 */
void calculateF() {
    // prepare the current particles for next iteration
    for (auto& p : particles) {
        p->nextIteration();
    }

    // iterate over the particle pairs
    bool reachedParticle = false;
    for (int i = 0; i < particles.size(); i++) {
        for (int j = i + 1; j < particles.size(); j++) {
            // check if particle combination has not been calculated
            auto p1 = particles[i];
            auto p2 = particles[j];

            // calculate force
            std::array<double, 3> force_p1_to_p2 = p1->calculateForce(*p2);
            // add to 1st particle
            p1->addF(force_p1_to_p2);

            // Usage of Newton's 3rd law
            for (int i = 0; i < 3; i++) {
                force_p1_to_p2[i] *= -1;
            }

            // add inverse to second particle
            p2->addF(force_p1_to_p2);
        }
    }
}

/**
 * \brief
 *  Calculate the new x for every particle based on its force postition
 */
void calculateX() {
    for (auto& p : particles) {
        p->calculateX(delta_t);
    }
}

/**
 * \brief
 * Calculate the new v for every particle based on force and velocity
 */
void calculateV() {
    for (auto& p : particles) {
        p->calculateV(delta_t);
    }
}

void plotParticles(int iteration, outputType type) {
    std::string out_name("MD_vtk");

    if (type = vtk) {
        auto w = outputWriter::VTKWriter();
        w.plotParticles(particles, out_name, iteration);
    } else if (type = xyz) {
        auto w = outputWriter::XYZWriter();
        w.plotParticles(particles, out_name, iteration);
    }
}