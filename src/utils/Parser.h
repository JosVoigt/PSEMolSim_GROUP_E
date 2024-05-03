#pragma once

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <iostream>
#include <iterator>
#include <string>
using namespace std;

#include "force/Force.h"
#include "force/Planet.h"
#include "outputWriter/VTKWriter.h"
#include "outputWriter/Writer.h"
#include "outputWriter/XYZWriter.h"

double const DEFAULT_DELTA = 0.00001;
double const DEFAULT_END = 1;

struct options {
    double delta_t;
    double start;
    double end;
    std::string filepath;
    std::string outfile;
    Writer* writer_;
    Force* force_;
};

/**
 *\brief
 *   parses the program options for the molecular simulation
 *\param ac
 *   The amount of arguments (usally called argc) provided to the main function
 *\param av
 *   The arguments (usally called argv) provided to the main function
 *\return an options struct containing the read options and settings
 */
options parse(int ac, char* av[]) {
    try {
        options o;

        po::options_description desc("Allowed options");

        desc.add_options()("help,h", "produce help message")(
            "delta,dt",
            po::value<double>(&o.delta_t)->default_value(DEFAULT_DELTA),
            "set step size")(
            "start,s", po::value<double>(&o.start)->default_value(0),
            "sets the recording start point for the simulation")(
            "end,e", po::value<double>(&o.end)->default_value(DEFAULT_END),
            "set end point")("file,f", po::value<std::string>(&o.filepath),
                             "set the path to the file containing initial "
                             "state of the molecules")(
            "outformat,of", po::value<std::string>()->default_value("vtk"),
            "set the output method")(
            "outfile,o",
            po::value<std::string>(&o.outfile)->default_value("simulation"),
            "set the output file name")(
            "type,t", po::value<std::string>()->default_value("planet"),
            "sets the type of particle");

        po::variables_map vm;
        po::store(po::parse_command_line(ac, av, desc), vm);
        po::notify(vm);

        // print help if required
        if (vm.count("help")) {
            cout << desc << "\n";
            std::exit(0);
        }

        // check if file is present
        if (!vm.count("file")) {
            std::cerr << "error: option '--file' required but missing"
                      << std::endl;
            std::exit(1);
        }

        if (vm["outformat"].as<string>() == "vtk") {
            auto w = outputWriter::VTKWriter();
            o.writer_ = (Writer*)&w;
        } else if (vm["output"].as<string>() == "xyz") {
            outputWriter::XYZWriter w = outputWriter::XYZWriter();
            o.writer_ = (Writer*)&w;
        } else {
            std::cerr << vm["output"].as<string>()
                      << " is not a valid output type" << std::endl;
            exit(1);
        }

        if (vm["type"].as<std::string>() == "planet") {
            Planet pl = Planet();
            o.force_ = (Force*)&pl;
        } else {
            std::cerr << vm["type"].as<string>() << " is not a valid type"
                      << std::endl;
            exit(1);
        }

        return o;

    } catch (exception& e) {
        cerr << "error: " << e.what() << "\n";
        std::exit(1);
    } catch (...) {
        cerr << "Exception of unknown type!\n";
        std::exit(1);
    }
}
