#pragma once

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <iostream>
#include <iterator>
#include <string>
using namespace std;

#include "VTKWriter.h"
#include "XYZWriter.h"
#include "writer.h"

double const DEFAULT_DELTA = 0.00001;
double const DEFAULT_END = 1;

struct options {
    double delta_t;
    double end;
    std::string filepath;
    writer* output_method;
};

options parse(int ac, char* av[]) {
    try {
        options o;

        po::options_description desc("Allowed options");

        desc.add_options()
        ("help,h", "produce help message")
        ("delta,dt",po::value<double>(&o.delta_t)->default_value(DEFAULT_DELTA),"set step size")
        ("end,e", po::value<double>(&o.end)->default_value(DEFAULT_END),"set end point")
        ("file,f", po::value<std::string>(&o.filepath),"set the path to the file containing initial state of the molecules")
        ("output,o", po::value<std::string>()->default_value("vtk"),"set the output method");

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

        if (vm["output"].as<string>() == "vtk") {
            auto w = outputWriter::VTKWriter();
            o.output_method = (writer*)&w;
        } else if (vm["output"].as<string>() == "xyz") {
            auto w = outputWriter::XYZWriter();
            o.output_method = (writer*)&w;
        }else {
            std::cerr << vm["output"].as<string>() << " is not a valid output type" << std::endl;
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
