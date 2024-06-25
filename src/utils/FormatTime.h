#pragma once

#include <string>

/**
 * @brief Formats the given seconds into a string of the form "HH:MM:SS"
 *
 * @param total_seconds Seconds to be formatted
 * @return std::string Formatted string
 *
 * If the given seconds are negative, "N/A" is returned.
 */
std::string format_seconds_eta(int total_seconds);

/**
 * @brief Formats the given seconds into a string of the form "HHh MMm SSs MMMms"
 *
 * @param total_seconds Seconds to be formatted
 * @return std::string Formatted string
 *
 * If the given seconds are negative, "N/A" is returned.
 */
std::string format_seconds_total(double total_seconds);
