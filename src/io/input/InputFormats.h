#pragma once

#include <set>
#include <string>

/**
 * @brief Returns a list of supported input file extensions
 *
 * @return std::vector<std::string> List of supported input file extensions
 *
 * Returns a list of supported input file extensions.
 * Supported file formats are:
 * For more information about the output file formats, see \ref InputFileFormats "Input File Formats"
 */
const std::set<std::string> get_supported_input_file_extensions();