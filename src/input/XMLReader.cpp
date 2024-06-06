#include "XMLReader.h"
#include <fstream>
#include <iostream>
#include <sstream>

#include "force/LennardJonesMolecule.h"
#include "utils/Parser.h"

XMLReader::XMLReader(const char* filename_) : filename(filename_) {}

void XMLReader::readData (parser::options &options) const {
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

    if (std::getline(iss, key, ':') && std::getline(iss, value)) {
      if (key == "Velocity") {
        std::stringstream iss_v(value);
        char ignore;
        double x_v, y_v, z_v;
        iss_v >> ignore >> ignore >> x_v >> ignore >> ignore >> ignore >> y_v >> ignore >> ignore >> ignore >> z_v;
        velocity[0] = x_v;
        velocity[1] = y_v;
        velocity[2] = z_v;
      } else if (key == "LowerLeftCorner") {
        std::stringstream iss_l(value);
        char ignore;
        double x_l, y_l, z_l;
        iss_l >> ignore >> ignore >> x_l >> ignore >> ignore >> ignore >> y_l >> ignore >> ignore >> ignore >> z_l;
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
        options.cuboids.emplace_back(x, y, z, distance, mass, std::stod(value),
                                     lowerLeftCorner, velocity);
      } else if (key == "Epsilon") {
        epsilon = std::stod(value);
      } else if (key == "Sigma") {
        options.force_ =
            std::make_shared<LennardJonesMolecule>(epsilon, std::stod(value));
      } else if (key == "Delta") {
        options.delta_t = std::stod(value);
      } else if (key == "Frequency") {
        options.writeoutFrequency = std::stoi(value);
      } else if (key == "Start") {
        options.start = std::stod(value);
      } else if (key == "End") {
        options.end = std::stod(value);
      } else if (key == "Outfile") {
        value.erase(0, 1);
        options.outfile = value;
      }
    }
  }

  file.close();
}
