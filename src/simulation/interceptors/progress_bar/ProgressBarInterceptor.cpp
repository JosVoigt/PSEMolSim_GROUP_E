#include "ProgressBarInterceptor.h"

#include <spdlog/fmt/chrono.h>
#include <sys/ioctl.h>

#include <filesystem>
#include <iostream>

#include "io/logger/Logger.h"
#include "simulation/SimulationParams.h"
#include "utils/FormatTime.h"

void printProgress(const std::filesystem::path& input_file_path, size_t percentage, size_t iteration, size_t expected_iterations,
                   int estimated_remaining_seconds, bool finished = false) {
    struct winsize size {};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

    auto should_progress_bar_length = std::min(std::max(size.ws_col - 90, 0), 100);
    auto length_full_progress_bar = static_cast<size_t>(percentage * static_cast<double>(should_progress_bar_length) / 100.0);

    auto progress_bar = fmt::format("[{}{}]", ansi_blue_bold + std::string(length_full_progress_bar, '#'),
                                    std::string(should_progress_bar_length - length_full_progress_bar, ' ') + ansi_end);

    std::string line = fmt::format("{} Step: {}/{} {:>3}%, ETA: {} [{}]", progress_bar, iteration, expected_iterations, percentage,
                                   format_seconds_eta(estimated_remaining_seconds), std::filesystem::path(input_file_path).stem().string());

    if (line.length() > size.ws_col) {
        line = line.substr(0, size.ws_col - 3) + "...";
    }

    std::cout << "\33[2K\r" << line << std::flush;

    if (finished) {
        std::cout << std::endl;
    }
}

void ProgressBarInterceptor::onSimulationStart(Simulation& simulation) {
    t_start = std::chrono::high_resolution_clock::now();
    t_prev = t_start;

    expected_iterations = static_cast<size_t>(std::ceil(simulation.params.end_time / simulation.params.delta_t) + 1);

    SimulationInterceptor::every_nth_iteration = std::max(1, static_cast<int>(expected_iterations / 100));

    printProgress(simulation.params.input_file_path, 0, 0, expected_iterations, -1);
}

void ProgressBarInterceptor::operator()(size_t iteration, Simulation& simulation) {
    // calculate time since last write
    auto t_now = std::chrono::high_resolution_clock::now();
    const double seconds_since_last_write = std::chrono::duration<double>(t_now - t_prev).count();
    t_prev = t_now;

    // calculate estimated remaining time
    const int estimated_remaining_seconds = std::floor(seconds_since_last_write * static_cast<double>(expected_iterations - iteration) /
                                                       static_cast<double>(every_nth_iteration));

    const size_t percentage =
        std::min(100ul, static_cast<size_t>(100.0 * static_cast<double>(iteration) / static_cast<double>(expected_iterations)));

    printProgress(simulation.params.input_file_path, percentage, iteration, expected_iterations, estimated_remaining_seconds);
}

void ProgressBarInterceptor::onSimulationEnd(size_t iteration, Simulation& simulation) {
    printProgress(simulation.params.input_file_path, 100, expected_iterations, expected_iterations, 0, true);
}

void ProgressBarInterceptor::logSummary(int depth) const {
    std::string indent = std::string(depth * 2, ' ');

    Logger::logger->info("{}╟┤{}ProgressBar: {}", indent, ansi_orange_bold, ansi_end);
    Logger::logger->info("{}║  ├Enabled", indent);
}

ProgressBarInterceptor::operator std::string() const { return ""; }