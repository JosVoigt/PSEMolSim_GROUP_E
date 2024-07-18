#pragma once

#include <spdlog/spdlog.h>

#include <memory>
#include <string>

const std::string ansi_blue_bold = "\e[34m\e[1m";
const std::string ansi_orange_bold = "\e[38;5;208m\e[1m";
const std::string ansi_yellow_bold = "\e[33m\e[1m";
const std::string ansi_bright_white_bold = "\e[97m\e[1m";
const std::string ansi_end = "\e[0m";

/**
 * @brief Class as wrapper and initializer for a globally usable logger
 */
class Logger {
   public:
    /**
     * @brief Enum for the type of logger
     */
    enum class LogType { FILE, STD };

    /**
     * @brief Initializes the logger
     */
    static std::shared_ptr<spdlog::logger> init_logger(LogType log_type = LogType::STD);

    /**
     * @brief Publically accessible shared pointer to the logger
     *
     * @return Logger
     */
    static std::shared_ptr<spdlog::logger> logger;

    /**
     * @brief Sets the log level of the logger
     *
     * @param log_level The log level to set
     */
    static void update_level(std::string& log_level);
};