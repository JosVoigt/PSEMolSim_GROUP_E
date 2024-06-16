#include "XMLReader.h"

#include <spdlog/spdlog.h>

#include <fstream>
#include <sstream>

#include "container/LinkedCellContainer.h"
#include "force/LennardJonesForce.h"
#include "outputWriter/VTKWriter.h"
#include "utils/Parser.h"

XMLReader::XMLReader(const char *filename_) : filename(filename_) {}

void XMLReader::readData(parser::options &options) const {
  std::ifstream file(filename);
  if (!file.is_open()) {
    spdlog::get("console")->critical("Unable to open file: ", filename);
    return;
  }

  std::string line;

  std::string key;
  std::string value;
  std::array<double, 3> velocity{};
  std::array<double, 3> lowerLeftCorner{};
  std::array<double, 3> center{};
  std::array<double, 3> velocity_d{};
  double distance, mass, x, y, z, epsilon;
  int radius;
  double mass_disc, distance_disc;
  int amountCellsX, amountCellsY, amountCellsZ;
  double maxChangeRate;

  options.writer_ = std::make_shared<outputWriter::VTKWriter>();

  while (std::getline(file, line)) {
    std::istringstream iss(line);

    if (std::getline(iss, key, ':') && std::getline(iss, value)) {
      if (key == "Velocity") {
        std::stringstream iss_v(value);
        char ignore;
        double x_v, y_v, z_v;
        iss_v >> ignore >> ignore >> x_v >> ignore >> ignore >> ignore >> y_v >>
            ignore >> ignore >> ignore >> z_v;
        velocity[0] = x_v;
        velocity[1] = y_v;
        velocity[2] = z_v;
      } else if (key == "LowerLeftCorner") {
        std::stringstream iss_l(value);
        char ignore;
        double x_l, y_l, z_l;
        iss_l >> ignore >> ignore >> x_l >> ignore >> ignore >> ignore >> y_l >>
            ignore >> ignore >> ignore >> z_l;
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
            std::make_shared<LennardJonesForce>(epsilon, std::stod(value));
      } else if (key == "Delta") {
        options.delta_t = std::stod(value);
      } else if (key == "Frequency") {
        options.writeoutFrequency = std::stoi(value);
      } else if (key == "Dimensions") {
        options.dimensions = std::stoi(value);
      } else if (key == "Start") {
        options.start = std::stod(value);
      } else if (key == "End") {
        options.end = std::stod(value);
      } else if (key == "Outfile") {
        value.erase(0, 1);
        options.outfile = value;
      } else if (key == "Center") {
        std::stringstream iss_c(value);
        char ignore;
        double x_c, y_c, z_c;
        iss_c >> ignore >> ignore >> x_c >> ignore >> ignore >> ignore >> y_c >>
            ignore >> ignore >> ignore >> z_c;
        center[0] = x_c;
        center[1] = y_c;
        center[2] = z_c;
      } else if (key == "Radius") {
        radius = std::stoi(value);
      } else if (key == "Mass_Disc") {
        mass_disc = std::stod(value);
      } else if (key == "Distance_Disc") {
        distance_disc = std::stod(value);
      } else if (key == "Velocity_Disc") {
        std::stringstream iss_vd(value);
        char ignore;
        double x_vd, y_vd, z_vd;
        iss_vd >> ignore >> ignore >> x_vd >> ignore >> ignore >> ignore >>
            y_vd >> ignore >> ignore >> ignore >> z_vd;
        velocity_d[0] = x_vd;
        velocity_d[1] = y_vd;
        velocity_d[2] = z_vd;
        options.discs.emplace_back(radius, distance_disc, mass_disc, velocity_d,
                                   center);
      }
    } else if (key == "AmountCellsX") {
      amountCellsX = std::stoi(value);
    } else if (key == "AmountCellsY") {
      amountCellsY = std::stoi(value);
    } else if (key == "AmountCellsZ") {
      amountCellsZ = std::stoi(value);
    } else if (key == "Sidelength") {
      std::array<LinkedCellContainer::BoundaryConditions, 6> conditions = {
          LinkedCellContainer::outflow, LinkedCellContainer::outflow,
          LinkedCellContainer::outflow, LinkedCellContainer::outflow,
          LinkedCellContainer::outflow, LinkedCellContainer::outflow};
      options.container_ = std::make_shared<LinkedCellContainer>(
          amountCellsX, amountCellsY, amountCellsZ, std::stod(value),
          conditions);
    } else if (key == "MaxChangeRate") {
        maxChangeRate = std::stod(value);
    } else if (key == "Dimensions_Thermostat") {
        options.thermostat_ = std::make_shared<Thermostat>(maxChangeRate,std::stoi(value));
    }
  }

  file.close();
}
