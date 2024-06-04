#include "XMLReader.h"
#include <fstream>
#include <sstream>
#include <iostream>

#include "../utils/Parser.h"
#include "../force/LennardJonesMolecule.h"

XMLReader ::XMLReader(const char* filename_) : filename(filename_) {}

void XMLReader::readData(parser::options &options) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }

    std::string line;

    std::string key;
    std::string value;
    double epsilon;
    std::array<double, 3> velocity{};
    std::array<double, 3> lowerLeftCorner{};
    double distance, mass, x, y, z;

    while (std::getline(file, line)) {

        std::istringstream iss(line);

        if (std::getline(iss, key, ':') && iss >> value) {
            if (key == "Velocity") {
                std::istringstream iss_v(value);
                char discard;
                double x_v, y_v, z_v;
                iss_v >> discard >> discard >> x_v >> discard >> discard >> y_v >> discard >> discard >> z_v;
                velocity[0] = x_v;
                velocity[1] = y_v;
                velocity[2] = z_v;
            } else if (key == "LowerLeftCorner") {
                std::istringstream iss_l(value);
                char discard;
                double x_l, y_l, z_l;
                iss_l >> discard >> discard >> x_l >> discard >> discard >> y_l >> discard >> discard >> z_l;
                lowerLeftCorner[0] = x_l;
                lowerLeftCorner[1] = y_l;
                lowerLeftCorner[2] = z_l;
            } else if (key == "Distance") {
                distance = std::stod(value);
            } else if (key == "Mass") {
                mass = std::stod(value);
            } else if (key == "X") {
                x = std::stod(value);
            } else if (key == "Y") {
                y = std::stod(value);
            } else if (key == "Z") {
                z = std::stod(value);
            } else if (key == "BrownianMotionMean") {
                options.cuboids.emplace_back(x, y, z, distance, mass, std::stod(value), lowerLeftCorner, velocity);
            } else if (key == "Epsilon") {
                epsilon = std::stod(value);
            } else if (key == "Sigma") {
                options.force_ = std::make_shared<LennardJonesMolecule>(epsilon, std::stod(value));
            } else if (key == "Delta") {
                options.delta_t = std::stod(value);
            } else if (key == "Frequency") {
                options.writeoutFrequency = std::stoi(value);
            } else if (key == "Start") {
                options.start = std::stod(value);
            } else if (key == "End") {
                options.end = std::stod(value);
            } else if (key == "Outfile") {
                options.outfile = value;
            }
        }
    }

    file.close();
}
