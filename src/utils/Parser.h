#pragma once

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <iostream>
#include <iterator>
#include <string>
using namespace std;

double const DEFAULT_DELTA = 0.00001;
double const DEFAULT_END = 1;

struct options {
    double delta_t;
    double end;
    std::string filepath;
};

options parse(int ac, char* av[]) {
    try {
        options o;

        po::options_description desc("Allowed options");

        desc.add_options()
            ("help,h", "produce help message")
            ("delta,dt", po::value<double>(&o.delta_t)->default_value(DEFAULT_DELTA), "set step size")
            ("end,e", po::value<double>(&o.end)->default_value(DEFAULT_END), "set end point")
            ("file,f", po::value<std::string>(&o.filepath), "set the path to the file containing initial state of the molecules");

        po::variables_map vm;
        po::store(po::parse_command_line(ac, av, desc), vm);
        po::notify(vm);

        // print help if required
        if (vm.count("help")) {
            cout << desc << "\n";
            std::exit(0);
        }

        std::cout << o.filepath << std::endl;

        // check if file is present
        if (!vm.count("file")) {
            std::cerr << "error: option '--file' required but missing" << std::endl;
            std::exit(1);
        }

        return o;

    }
    catch (exception& e) {
        cerr << "error: " << e.what() << "\n";
        std::exit(1);
    }
    catch (...) {
        cerr << "Exception of unknown type!\n";
        std::exit(1);
    }
}
