#include "OutputFormats.h"

#include "io/logger/Logger.h"

const std::map<std::string, OutputFormat> get_supported_output_formats() {
    return {{"vtu", OutputFormat::VTU}, {"xyz", OutputFormat::XYZ}, {"chkpt", OutputFormat::CHKPT}};
}

OutputFormat convertToOutputFormat(const std::string& output_format) {
    auto supported = get_supported_output_formats();

    if (!supported.contains(output_format)) {
        auto supported_formats = std::string();
        for (auto& [name, format] : supported) {
            supported_formats += name + ", ";
        }

        Logger::logger->error("Invalid output format given: {}. Supported output formats are: {}", output_format, supported_formats);
        throw std::runtime_error("Invalid output format given");
    }

    return supported[output_format];
}