#pragma once

#include <map>
#include <string>

/**
 * @brief Enum class to specify the output format
 */
enum class OutputFormat { VTU, XYZ, CHKPT };

/**
 * @brief Returns a mappping of supported output formats
 *
 * @return std::map<std::string,OutputFormat> Mapping of supported output formats
 */
const std::map<std::string, OutputFormat> get_supported_output_formats();

/**
 * @brief Converts a string to an OutputFormat
 *
 * @param output_format The string to convert
 * @return OutputFormat The converted OutputFormat
 */
OutputFormat convertToOutputFormat(const std::string& output_format);