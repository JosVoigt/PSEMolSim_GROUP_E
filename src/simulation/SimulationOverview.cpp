#include "SimulationOverview.h"

#include "io/logger/Logger.h"
#include "simulation/SimulationParams.h"
#include "utils/FormatTime.h"

void SimulationOverview::logSummary(int depth) const {
    std::string indent = std::string(depth * 2, ' ');

    Logger::logger->info("{}╔════════════════════════════════════════", indent);
    Logger::logger->info("{}╟┤{}Simulation overview: {}", indent, ansi_yellow_bold, ansi_end);
    Logger::logger->info("{}║  Input file: {}", indent, params.input_file_path.string());
    Logger::logger->info("{}║  Output directory: {}", indent, params.output_dir_path.string());
    Logger::logger->info("{}║  Simulation time: {}", indent, format_seconds_total(total_time_seconds));
    Logger::logger->info("{}║  Number of iterations: {}", indent, total_iterations);
    Logger::logger->info("{}║  Number of particles left: {}", indent, resulting_particles.size());
    Logger::logger->info("{}╟┤{}Interceptor Logs: {}", indent, ansi_yellow_bold, ansi_end);

    if (interceptor_summaries.empty()) {
        Logger::logger->info("{}║   └No interceptors", indent);
    } else {
        for (auto& interceptor_summary : interceptor_summaries) {
            Logger::logger->info("{}║   ├{}", indent, interceptor_summary);
        }
    }

    Logger::logger->info("{}╚════════════════════════════════════════", indent);
}
