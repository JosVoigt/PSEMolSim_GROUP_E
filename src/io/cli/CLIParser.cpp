#include "CLIParser.h"

#include <boost/program_options.hpp>
#include <limits>

#include "io/logger/Logger.h"
#include "spdlog/sinks/rotating_file_sink.h"

CLIParams parse_arguments(int argc, char* argsv[]) {
    std::filesystem::path input_file_path;
    std::string log_level;
    std::string log_output;

    bool performance_test = false;
    bool fresh = false;

    // choosing 0 as one of the parameters (end_time, delta_t, fps, video_length) is equivalent to choosing the default value
    boost::program_options::options_description options_desc("Allowed options");
    options_desc.add_options()("help,h", "produce help message");
    options_desc.add_options()(
        "input_file_path,f", boost::program_options::value<std::filesystem::path>(&input_file_path),
        "The path to the input file. Must be specified, otherwise the program will terminate. Can be inserted as positional argument.");
    options_desc.add_options()("log_level,l", boost::program_options::value<std::string>(&log_level)->default_value("info"),
                               "The log level. Possible values: trace, debug, info, warning, error, critical, off");
    options_desc.add_options()("performance_test,p", "Run the simulation in performance test mode");
    options_desc.add_options()(
        "log_output", boost::program_options::value<std::string>(&log_output)->default_value("std"),
        "You can only choose between the output options std(only cl output) and file (only file output). Default: no file output");
    options_desc.add_options()(
        "fresh", boost::program_options::bool_switch(&fresh)->default_value(false),
        "Rerun the simulation from scratch without using any cached data. This will delete the whole output directory.");

    boost::program_options::positional_options_description positional_options_desc;
    positional_options_desc.add("input_file_path", -1);

    boost::program_options::variables_map variables_map;
    boost::program_options::store(
        boost::program_options::command_line_parser(argc, argsv).options(options_desc).positional(positional_options_desc).run(),
        variables_map);
    boost::program_options::notify(variables_map);

    if (log_output == "std" || log_output == "STD") {
        Logger::logger->info("Log output: std");
    } else if (log_output == "file" || log_output == "FILE") {
        Logger::logger = Logger::init_logger(Logger::LogType::FILE);
        Logger::logger->info("Log output: file");
    } else {
        std::cout << "Error: Invalid log output given. Options: no file output: 'std' and file output: 'file'" << std::endl;
        exit(-1);
    }

    Logger::update_level(log_level);

    if (argc <= 1 || variables_map.count("help")) {
        std::stringstream help_message;
        help_message << options_desc << std::endl;
        Logger::logger->info(help_message.str());
        exit(-1);
    }
    if (!variables_map.count("input_file_path")) {
        Logger::logger->error("No input file path given.");
        std::stringstream help_message;
        help_message << options_desc << std::endl;
        Logger::logger->info(help_message.str());
        exit(-1);
    }
    if (variables_map.count("performance_test")) {
        performance_test = true;
    }

    return CLIParams{input_file_path, performance_test, fresh};
}

SimulationParams merge_parameters(const CLIParams& params_cli, const std::optional<SimulationParams>& file_params) {
    if (!file_params) {
        Logger::logger->warn("No simulation parameters provided. Try using a XML file as input.");
        throw std::runtime_error("No simulation parameters provided. Try using a XML file as input.");
    }

    SimulationParams params = *file_params;

    // Update the parameters with the ones from the command line
    params.fresh = params_cli.fresh;
    params.performance_test = params_cli.performance_test;

    return params;
}