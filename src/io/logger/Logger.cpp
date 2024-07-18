#include "Logger.h"

#include <filesystem>
#include <iostream>

#include "spdlog/async.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Logger::logger = init_logger();

void createDirectory(const std::string& dir_name) {
    if (!std::filesystem::exists(dir_name)) {
        try {
            std::filesystem::create_directory(dir_name);
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error creating directory: " << e.what() << std::endl;
            throw e;
        }
    }
}

std::shared_ptr<spdlog::logger> Logger::init_logger(LogType log_type) {
    auto standard_out = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    standard_out->set_color(spdlog::level::critical, standard_out->dark);
    standard_out->set_color(spdlog::level::err, standard_out->red);
    standard_out->set_color(spdlog::level::warn, standard_out->yellow);
    standard_out->set_color(spdlog::level::info, standard_out->green);
    standard_out->set_color(spdlog::level::debug, standard_out->blue);
    standard_out->set_color(spdlog::level::trace, standard_out->magenta);

    spdlog::init_thread_pool(8192, 1);
    std::shared_ptr<spdlog::logger> new_logger;
    if (log_type == LogType::FILE) {
        createDirectory("logs");
        new_logger = spdlog::rotating_logger_st("file_logger", "logs/log", 1048576 * 5, 3);
    } else {
        new_logger =
            std::make_shared<spdlog::async_logger>("std_logger", standard_out, spdlog::thread_pool(), spdlog::async_overflow_policy::block);
    }

    new_logger->set_level(spdlog::level::off);
    new_logger->set_pattern("[%H:%M:%S] %^[%l]%$ %v");

    return new_logger;
}

void Logger::update_level(std::string& log_level) {
    if (log_level == "trace") {
        Logger::logger->set_level(spdlog::level::trace);
    } else if (log_level == "debug") {
        Logger::logger->set_level(spdlog::level::debug);
    } else if (log_level == "info") {
        Logger::logger->set_level(spdlog::level::info);
    } else if (log_level == "warning") {
        Logger::logger->set_level(spdlog::level::warn);
    } else if (log_level == "error") {
        Logger::logger->set_level(spdlog::level::err);
    } else if (log_level == "critical") {
        logger->set_level(spdlog::level::critical);
    } else if (log_level == "off") {
        Logger::logger->set_level(spdlog::level::off);
    } else {
        std::cout << "Error: Invalid log level given." << std::endl;
        throw std::invalid_argument("Invalid log level given.");
    }
}
