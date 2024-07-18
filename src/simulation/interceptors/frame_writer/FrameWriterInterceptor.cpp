#include "FrameWriterInterceptor.h"

#include <cmath>
#include <string>

#include "io/logger/Logger.h"
#include "io/output/OutputFormats.h"

FrameWriterInterceptor::FrameWriterInterceptor(OutputFormat output_format, int fps, int video_length)
    : output_format(output_format), fps(fps), video_length(video_length) {
    if (fps < 0) {
        Logger::logger->error("FPS must be positive");
        throw std::runtime_error("FPS must be positive");
    }
    if (video_length < 0) {
        Logger::logger->error("Video length must be positive");
        throw std::runtime_error("Video length must be positive");
    }
}

void FrameWriterInterceptor::onSimulationStart(Simulation& simulation) {
    auto expected_iterations = static_cast<size_t>(std::ceil(simulation.params.end_time / simulation.params.delta_t) + 1);

    FrameWriterInterceptor::every_nth_iteration = std::max(expected_iterations / (fps * video_length), 1ul);

    file_output_handler = std::make_unique<FileOutputHandler>(output_format, simulation.params);

    file_output_handler->writeFile(0, simulation.particle_container);
    file_counter++;
}

void FrameWriterInterceptor::operator()(size_t iteration, Simulation& simulation) {
    file_output_handler->writeFile(static_cast<int>(iteration), simulation.particle_container);
    file_counter++;
}

void FrameWriterInterceptor::onSimulationEnd(size_t iteration, Simulation& simulation) {
    file_output_handler->writeFile(static_cast<int>(iteration), simulation.particle_container);
    file_counter++;
}

void FrameWriterInterceptor::logSummary(int depth) const {
    std::string indent = std::string(depth * 2, ' ');

    auto supported_output_formats = get_supported_output_formats();

    std::string output_format_s =
        std::find_if(supported_output_formats.begin(), supported_output_formats.end(), [this](const auto& format) {
            return format.second == output_format;
        })->first;

    Logger::logger->info("{}╟┤{}Frame Writer: {}", indent, ansi_orange_bold, ansi_end);
    Logger::logger->info("{}║  ┌Output format: {}", indent, output_format_s);
    Logger::logger->info("{}║  ├Frames per second: {}", indent, fps);
    Logger::logger->info("{}║  └Video length: {}", indent, video_length);
}

FrameWriterInterceptor::operator std::string() const { return "Frame Writer: " + std::to_string(file_counter) + " files saved"; }
