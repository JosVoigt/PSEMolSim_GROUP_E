#pragma once

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <spdlog/spdlog.h>

#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "force/Force.h"
#include "force/LennardJonesMolecule.h"
#include "force/Planet.h"
#include "outputWriter/VTKWriter.h"
#include "outputWriter/Writer.h"
#include "outputWriter/XYZWriter.h"
#include "input/CuboidGenerator.h"

double const DEFAULT_DELTA = 0.00001;
double const DEFAULT_END = 1;
int const DEFAULT_FREQUENCY = 10;

struct options {
    bool executeTests = false;
    double delta_t{};
    double start{};
    double end{};
    int writeoutFrequency{};
    spdlog::level::level_enum loglevel;
    std::vector<std::string> filepath;
    std::vector<CuboidGenerator> cuboids;
    std::string outfile;
    std::shared_ptr<Writer> writer_;
    std::shared_ptr<Force> force_;
};


//predeclaration
void parseCuboids(std::string cuboid_s, std::vector<CuboidGenerator>&ret);

/**
 *\brief
 *   parses the program options for the molecular simulation
 *
 *   If tests should be executed all other arguments should be expected to be
 *empty \param ac The amount of arguments (usually called argc) provided to the
 *main function \param av The arguments (usually called argv) provided to the
 *main function \return an options struct containing the read options and
 *settings
 */
options parse(int ac, char* av[]) {
    try {
        options opts;

        opts.cuboids = std::vector<CuboidGenerator>();

        po::options_description desc("Allowed options");

        desc.add_options()
            ("help,h", "produce help message, ignores all other flags")
            ("outlevel,l", po::value<std::string>()->default_value("info"), ("the output level of information (info, err, critical, debug, off)"))
            ("test,t", "execute tests, ignores all other flags except help")
            ("cuboid,c", po::value<std::string>(), "accepts the generation paramaters for the cuboids")
            ("delta,d",po::value<double>(&opts.delta_t)->default_value(DEFAULT_DELTA),"set step size")
            ("frequency,f", po::value<int>(&opts.writeoutFrequency)->default_value(DEFAULT_FREQUENCY),"sets the frequency for data writeout")
            ("start,s", po::value<double>(&opts.start)->default_value(0),"sets the recording start point for the simulation")
            ("end,e", po::value<double>(&opts.end)->default_value(DEFAULT_END),"set end point")
            ("file,F",po::value<std::vector<std::string>>(&opts.filepath)->multitoken(),"set the path to the file(s) containing initial state of the molecules")
            ("outformat,O",po::value<std::string>()->default_value("vtk"),"set the output method (vtk,xyz)")
            ("outfile,o",po::value<std::string>(&opts.outfile)->default_value("simulation"),"set the output file name")
            ("planet","sets particle mode to planet, exclusive with other particle modes")
            ("lenjonesmol", po::value<std::vector<double>>()->multitoken(),"set particle mode to molecules using Lennard-Jones with epsilon and sigma as the following values, exclusive with other particle modes");


        po::variables_map vm;
        po::store(po::parse_command_line(ac, av, desc), vm);
        po::notify(vm);

        // print help if required
        if (vm.count("help")) {
            std::cout << desc << "\n";
            std::exit(0);
        }

        opts.executeTests = (vm.count("test"));

        if (opts.executeTests) return opts;

        // check exclusivity and existence of force modes
        std::vector<double> ljm_args;
        if (vm.count("planet") && vm.count("lenjonesmol")) {
            std::cerr << "Please choose EXACTLY ONE force mode" << std::endl;
            exit(1);
        } else if (vm.count("planet")) {
            opts.force_ = std::shared_ptr<Force>(new Planet());
        } else if (!vm["lenjonesmol"].empty() &&
                   (ljm_args = vm["lenjonesmol"].as<std::vector<double>>())
                           .size() == 2) {
            opts.force_ = std::shared_ptr<Force>(
                new LennardJonesMolecule(ljm_args[0], ljm_args[1]));
        } else {
            std::cerr << "Please provide a single force mode" << std::endl;
            exit(1);
        }

        if (vm["outformat"].as<std::string>() == "vtk") {
            opts.writer_ =
                std::shared_ptr<Writer>(new outputWriter::VTKWriter());
        } else if (vm["output"].as<std::string>() == "xyz") {
            opts.writer_ =
                std::shared_ptr<Writer>(new outputWriter::XYZWriter());
        } else {
            std::cerr << vm["output"].as<std::string>()
                      << " is not a valid output type" << std::endl;
            exit(1);
        }

        //parse cuboid options
        if (vm.count("cuboid") != 0) {
            parseCuboids(vm["cuboid"].as<std::string>(), opts.cuboids);
        }

        //check if we got passed any particles
        if (opts.cuboids.empty() && opts.filepath.empty()) {
            spdlog::get("file")->critical("There are no particles to run the simulation on. Please include at least 2 particles via file or cuboid.");
            exit(1);
        }

        return opts;

    } catch (std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        std::exit(1);
    } catch (...) {
        std::cerr << "Exception of unknown type!\n";
        std::exit(1);
    }
}

// states of the automata parsing the string
enum class cuboid_parser_state {
    start,
    V_B,
    v1,
    v2,
    v3,
    V_E,
    LLFC_B,
    llfc1,
    llfc2,
    llfc3,
    LLFC_E,
    dist,
    mass,
    x,
    y,
    z,
    avgBrownMot,
    end,
    trap
};

std::string parser_state_tostring (cuboid_parser_state state) {
    switch (state)
    {
    case cuboid_parser_state::start:
        return "start";
    case cuboid_parser_state::V_B:
        return "V_B";
    case cuboid_parser_state::v1:
        return "v1";
    case cuboid_parser_state::v2:
        return "v2";
    case cuboid_parser_state::v3:
        return "v3";
    case cuboid_parser_state::V_E:
        return "V_E";
    case cuboid_parser_state::LLFC_B:
        return "LLFC_B";
    case cuboid_parser_state::llfc1:
        return "llfc1";
    case cuboid_parser_state::llfc2:
        return "llfc2";
    case cuboid_parser_state::llfc3:
        return "llfc3";
    case cuboid_parser_state::LLFC_E:
        return "LLFC_E";
    case cuboid_parser_state::dist:
        return "dist";
    case cuboid_parser_state::mass:
        return "mass";
    case cuboid_parser_state::x:
        return "x";
    case cuboid_parser_state::y:
        return "y";
    case cuboid_parser_state::z:
        return "z";
    case cuboid_parser_state::avgBrownMot:
        return "avgBrownMot";
    case cuboid_parser_state::end:
        return "end";
    case cuboid_parser_state::trap:
        return "trap";
    }
    return "NOT A STATE";
}

/**
 * \brief
 * Executes the DFA for the cuboid input string.
 * Input strings of the form
 * [[D,D,D],[D,D,D],D,D,I,I,I,D](,[[D,D,D],[D,D,D],D,D,I,I,I,D])*
 * are getting accepted.
 * D here means double, I integer.
 * It is assumed that Doubles and Integers only contain 0-9 '.' and '-'.
 * The square brackets are symbols, the round brackets belong to the REGEX.
 * 
 * The meaning is the velocity (3Dvector) the lower left corner (3D vector) the distance , the mass , x , y, z, and the mean of the brownian motion.
 * \param cuboid_s
 * The string to get parsed.
 * \return
 * The list of CuboidGenerators that have been extracted from the string
 */
void parseCuboids(std::string cuboid_s, std::vector<CuboidGenerator>& ret) {
    spdlog::get("file")->debug("INIT CUBOID_PARSER");
    spdlog::get("file")->debug("Cuboid parser received input string: " + cuboid_s);

    std::string currentString = "";
    cuboid_parser_state state = cuboid_parser_state::start;

    std::array<double, 3> velo;
    std::array<double, 3> llfc;
    double d;
    double m;
    double aBM;
    int xc;
    int yc;
    int zc;

    for (int i = 0; i < cuboid_s.length() && state != cuboid_parser_state::trap; i++) {
        char currentChar = cuboid_s.at(i);
        spdlog::get("file")->debug(parser_state_tostring(state) + " -> " + currentChar + " \n    Cached chars are: " + currentString);
        switch (state) {
            case cuboid_parser_state::start:
                if (currentChar == '[')
                    state = cuboid_parser_state::V_B;
                else
                    state = cuboid_parser_state::trap;
                break;
            case cuboid_parser_state::V_B:
                if (currentChar =='[') 
                    state = cuboid_parser_state::v1;
                else
                    state = cuboid_parser_state::trap;
                break;
            case cuboid_parser_state::v1:
                // is 0-9 or . or - and not /
                if (currentChar <= 57 && currentChar >= 45 && currentChar != 47)
                    currentString += currentChar;
                else if (currentChar == ',') {
                    velo[0] = stod(currentString);
                    state = cuboid_parser_state::v2;
                    currentString = "";
                } else
                    state = cuboid_parser_state::trap;
                break;
            case cuboid_parser_state::v2:
                // is 0-9 or . or - and not /
                if (currentChar <= 57 && currentChar >= 45 && currentChar != 47)
                    currentString += currentChar;
                else if (currentChar == ',') {
                    velo[1] = stod(currentString);
                    state = cuboid_parser_state::v3;
                    currentString = "";
                } else
                    state = cuboid_parser_state::trap;
                break;
            case cuboid_parser_state::v3:
                // is 0-9 or . or - and not /
                if (currentChar <= 57 && currentChar >= 45 && currentChar != 47)
                    currentString += currentChar;
                else if (currentChar == ']') {
                    velo[2] = stod(currentString);
                    state = cuboid_parser_state::V_E;
                    currentString = "";
                } else
                    state = cuboid_parser_state::trap;
                break;
            case cuboid_parser_state::V_E:
                if (currentChar == ',')
                    state = cuboid_parser_state::LLFC_B;
                else
                    state = cuboid_parser_state::trap;
                break;
            case cuboid_parser_state::LLFC_B:
                if (currentChar == '[')
                    state = cuboid_parser_state::llfc1;
                else
                    state = cuboid_parser_state::trap;
                break;
            case cuboid_parser_state::llfc1:
                // is 0-9 or . or - and not /
                if (currentChar <= 57 && currentChar >= 45 && currentChar != 47)
                    currentString += currentChar;
                else if (currentChar == ',') {
                    llfc[0] = stod(currentString);
                    state = cuboid_parser_state::llfc2;
                    currentString = "";
                } else
                    state = cuboid_parser_state::trap;
                break;
            case cuboid_parser_state::llfc2:
                // is 0-9 or . or - and not /
                if (currentChar <= 57 && currentChar >= 45 && currentChar != 47)
                    currentString += currentChar;
                else if (currentChar == ',') {
                    llfc[1] = stod(currentString);
                    state = cuboid_parser_state::llfc3;
                    currentString = "";
                } else
                    state = cuboid_parser_state::trap;
                break;
            case cuboid_parser_state::llfc3:
                // is 0-9 or . or - and not /
                if (currentChar <= 57 && currentChar >= 45 && currentChar != 47)
                    currentString += currentChar;
                else if (currentChar == ']') {
                    llfc[2] = stod(currentString);
                    state = cuboid_parser_state::LLFC_E;
                    currentString = "";
                } else
                    state = cuboid_parser_state::trap;
                break;
            case cuboid_parser_state::LLFC_E:
                if (currentChar == ',')
                    state = cuboid_parser_state::dist;
                else
                    state = cuboid_parser_state::trap;
                break;
            case cuboid_parser_state::dist:
                // is 0-9 or . or - and not /
                if (currentChar <= 57 && currentChar >= 45 && currentChar != 47)
                    currentString += currentChar;
                else if (currentChar == ',') {
                    d = stod(currentString);
                    state = cuboid_parser_state::mass;
                    currentString = "";
                } else
                    state = cuboid_parser_state::trap;
                break;
            case cuboid_parser_state::mass:
                // is 0-9 or . or - and not /
                if (currentChar <= 57 && currentChar >= 45 && currentChar != 47)
                    currentString += currentChar;
                else if (currentChar == ',') {
                    m = stod(currentString);
                    state = cuboid_parser_state::x;
                    currentString = "";
                } else
                    state = cuboid_parser_state::trap;
                break;
            case cuboid_parser_state::x:
                // is 0-9 or . or - and not /
                if (currentChar <= 57 && currentChar >= 45 && currentChar != 47)
                    currentString += currentChar;
                else if (currentChar == ',') {
                    xc = stoi(currentString);
                    state = cuboid_parser_state::y;
                    currentString = "";
                } else
                    state = cuboid_parser_state::trap;
                break;
            case cuboid_parser_state::y:
                // is 0-9 or . or - and not /
                if (currentChar <= 57 && currentChar >= 45 && currentChar != 47)
                    currentString += currentChar;
                else if (currentChar == ',') {
                    yc = stoi(currentString);
                    state = cuboid_parser_state::z;
                    currentString = "";
                } else
                    state = cuboid_parser_state::trap;
                break;
            case cuboid_parser_state::z:
                // is 0-9 or . or - and not /
                if (currentChar <= 57 && currentChar >= 45 && currentChar != 47)
                    currentString += currentChar;
                else if (currentChar == ',') {
                    zc = stoi(currentString);
                    state = cuboid_parser_state::avgBrownMot;
                    currentString = "";
                } else
                    state = cuboid_parser_state::trap;
                break;
            case cuboid_parser_state::avgBrownMot:
                // is 0-9 or . or - and not /
                if (currentChar <= 57 && currentChar >= 45 && currentChar != 47)
                    currentString += currentChar;
                else if (currentChar == ']') {
                    aBM = stod(currentString);
                    state = cuboid_parser_state::end;
                    currentString = "";
                } else
                    state = cuboid_parser_state::trap;
                break;
            case cuboid_parser_state::trap:
                // only here for exaustive matching should not be reachable
                spdlog::get("file")->debug("trap state has been reached. This should be impossible.");
                break;
            case cuboid_parser_state::end:
                std::stringstream ss;
                ss << "Emplacing back cuboid with args x:" << xc << " y:" << yc <<
                          " z:" << zc << " dist:" << d << " mass:" << m <<
                          " corner:" << llfc << " velocity" <<
                          velo;
                spdlog::get("file")->debug(ss.str());
                ret.emplace_back(xc, yc, zc, d, m, aBM,llfc, velo);
                if (currentChar == ',')
                    state = cuboid_parser_state::start;
                else
                    state = cuboid_parser_state::trap;
                break;
        }
    }
    spdlog::get("file")->debug("Parsing finished with state: " + parser_state_tostring(state));
    if (state != cuboid_parser_state::end) {
        spdlog::get("console")->critical("Not an accepted cuboid string!");
        exit(1);
    }
    else {
        std::stringstream ss;
        ss << "Emplacing back cuboid with args x:" << xc << " y:" << yc
           << " z:" << zc << " dist:" << d << " mass:" << m << " Brownian motion: " << aBM 
           << " corner:" << llfc << " velocity: " << velo;
        spdlog::get("file")->debug(ss.str());
        ret.emplace_back(xc, yc, zc, d, m, aBM, llfc, velo);
    }
}
