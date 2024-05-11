#pragma once

#include <boost/program_options.hpp>
namespace po = boost::program_options;

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
#include "logger/logger.h"

double const DEFAULT_DELTA = 0.00001;
double const DEFAULT_END = 1;
int const DEFAULT_FREQUENCY = 10;

struct options {
    bool executeTests = false;
    double delta_t{};
    double start{};
    double end{};
    int writeoutFrequency{};
    std::vector<std::string> filepath;
    std::vector<CuboidGenerator> cuboids;
    std::string outfile;
    std::shared_ptr<Writer> writer_;
    std::shared_ptr<Force> force_;
};


//predeclarration
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

        if (vm["outlevel"].as<std::string>() == "info") {
            spdlog::set_level(info);
        } else if (vm["outlevel"].as<std::string>() == "err") {
            spdlog::set_level(err);
        } else if (vm["outlevel"].as<std::string>() == "critical") {
            spdlog::set_level(critical);
        }
        else if (vm["outlevel"].as<std::string>() == "off") {
            spdlog::set_level(off);
        }
        else if (vm["outlevel"].as<std::string>() == "debug") {
            spdlog::set_level(debug);
        }
        else {
            spdlog::set_level(info);
            log_error(err, vm["outlevel"].as<std::string>() + " is not a valid logger mode, defaulted to info. The program will continue running.");
        }

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
            log_error(critical, "There are no particles to run the simulation on. Please include at least 2 particles via file or cuboid.");
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
enum cuboid_parser_state {
    start,
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
    end,
    trap
};

/**
 * \brief
 * Executes the NFA for the cuboid input string.
 * Input strings of the form
 * ( )*[[D,D,D],[D,D,D],D,D,I,I,I](,[[D,D,D],[D,D,D],D,D,I,I,I])*
 * are getting accepted
 * D here means double, I integer.
 * It is assumed that Doubles and Integers only contain 0-9 '.' and '-'.
 * The square brackets are symbols, the round brackets belong to the REGEX.
 * \param cuboid_s
 * The string to get parsed.
 * \return
 * The list of CuboidGenerators that have been extracted from the string
 */
void parseCuboids(std::string cuboid_s, std::vector<CuboidGenerator>& ret) {
    std::string currentString = "";
    cuboid_parser_state state = start;

    std::array<double, 3> velo;
    std::array<double, 3> llfc;
    double d;
    double m;
    int xc;
    int yc;
    int zc;

    for (int i = 0; i < cuboid_s.length() && state != trap; i++) {
        char currentChar = cuboid_s[i];
        log_file(debug, "" + currentChar);
        switch (state)
        {
        case start:
            if (currentChar == '[')
                state = v1;
            else if(currentChar == ' ')
                state = start;
            else
                state = trap;
            break;
        case v1:
            //is 0-9 or . or - and not /
            if (currentChar <= 57 && currentChar >= 45 && currentChar != 47) 
                currentString += currentChar;
            else if (currentChar == ',') {
                velo[0] = stod(currentString);
                state = v2;
                currentString = "";
            } 
            else
                state = trap;
            break;
        case v2:
            // is 0-9 or . or - and not /
            if (currentChar <= 57 && currentChar >= 45 && currentChar != 47)
                currentString += currentChar;
            else if (currentChar == ',') {
                velo[1] = stod(currentString);
                state = v3;
                currentString = "";
            }
            else
                state = trap;
            break;
        case v3:
            // is 0-9 or . or - and not /
            if (currentChar <= 57 && currentChar >= 45 && currentChar != 47)
                currentString += currentChar;
            else if (currentChar == ']') {
                velo[2] = stod(currentString);
                state = V_E;
                currentString = "";
            }
            else 
                state = trap;
            break;
        case V_E:
            if (currentChar == ',')
                state = LLFC_B;
            else
                state = trap;
            break;
        case LLFC_B:
            if (currentChar == '[')
                state = llfc1;
            else
                state = trap;
            break;
        case llfc1:
            // is 0-9 or . or - and not /
            if (currentChar <= 57 && currentChar >= 45 && currentChar != 47)
                currentString += currentChar;
            else if (currentChar == ',') {
                llfc[0] = stod(currentString);
                state = llfc2;
                currentString = "";
            } else
                state = trap;
            break;
        case llfc2:
            // is 0-9 or . or - and not /
            if (currentChar <= 57 && currentChar >= 45 && currentChar != 47)
                currentString += currentChar;
            else if (currentChar == ',') {
                llfc[1] = stod(currentString);
                state = llfc3;
                currentString = "";
            } else
                state = trap;
            break;
        case llfc3:
            // is 0-9 or . or - and not /
            if (currentChar <= 57 && currentChar >= 45 && currentChar != 47)
                currentString += currentChar;
            else if (currentChar == ']') {
                llfc[2] = stod(currentString);
                state = LLFC_E;
                currentString = "";
            } else
                state = trap;
            break;
        case LLFC_E:
            if (currentChar == ',')
                state = dist;
            else
                state = trap;
            break;
        case dist:
            // is 0-9 or . or - and not /
            if (currentChar <= 57 && currentChar >= 45 && currentChar != 47)
                currentString += currentChar;
            else if (currentChar == ',') {
                d = stod(currentString);
                state = mass;
                currentString = "";
            } else
                state = trap;
            break;
        case mass:
            // is 0-9 or . or - and not /
            if (currentChar <= 57 && currentChar >= 45 && currentChar != 47)
                currentString += currentChar;
            else if (currentChar == ',') {
                m = stod(currentString);
                state = mass;
                currentString = "";
            } else
                state = trap;
            break;
        case x:
            // is 0-9 or . or - and not /
            if (currentChar <= 57 && currentChar >= 45 && currentChar != 47)
                currentString += currentChar;
            else if (currentChar == ',') {
                xc = stoi(currentString);
                state = y;
                currentString = "";
            } else
                state = trap;
            break;
        case y:
            // is 0-9 or . or - and not /
            if (currentChar <= 57 && currentChar >= 45 && currentChar != 47)
                currentString += currentChar;
            else if (currentChar == ',') {
                d = stoi(currentString);
                state = z;
                currentString = "";
            } else
                state = trap;
            break;
        case z:
            // is 0-9 or . or - and not /
            if (currentChar <= 57 && currentChar >= 45 && currentChar != 47)
                currentString += currentChar;
            else if (currentChar == ']') {
                d = stoi(currentString);
                state = end;
                currentString = "";
            } else
                state = trap;
            break;
        case end:
            ret.emplace_back(x,y,z,d,m,llfc,velo);
            if (currentChar == ']')
                state = start;
            else
                state = trap;
            break;
        case trap:
        //only here for exaustive matchingm should not be reachable
            log_file (debug, "trap state has been reached. This should be impossible.");
            break;
        }
    }
    if (state != end) {
        log_error(critical, "Not an accepted cuboid string!");
        exit(1);
    }
}
