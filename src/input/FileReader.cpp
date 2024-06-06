/*
 * FileReader.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include "FileReader.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

FileReader::FileReader(const char* filename_) : filename(filename_) {}

void FileReader::readData(std::list<Particle>& particles, int dimensions) {
    std::array<double, 3> x{};
    std::array<double, 3> v{};
    double m;
    int num_particles = 0;

    std::ifstream input_file(filename);
    std::string tmp_string;

    if (input_file.is_open()) {
        getline(input_file, tmp_string);
        while (tmp_string.empty() or tmp_string[0] == '#') {
            getline(input_file, tmp_string);
        }

        std::istringstream numstream(tmp_string);
        numstream >> num_particles;
        getline(input_file, tmp_string);
        for (int i = 0; i < num_particles; i++) {
            std::istringstream datastream(tmp_string);

            for (auto& xj : x) {
                datastream >> xj;
            }
            for (auto& vj : v) {
                datastream >> vj;
            }
            if (datastream.eof()) {
                std::cerr << "Error reading file: eof reached unexpectedly "
                             "reading from line "
                          << i << std::endl;
                exit(-1);
            }
            datastream >> m;
            particles.emplace_back(x, v, m);

            getline(input_file, tmp_string);
        }
    } else {
        std::cerr << "Error: could not open file " << filename << std::endl;
        exit(-1);
    }
}
