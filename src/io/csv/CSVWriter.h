#pragma once

#include <array>
#include <filesystem>
#include <fstream>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "io/logger/Logger.h"

/**
 * @brief Writes a single element to the CSV file.
 *
 * @param file The file stream to write to.
 * @param value The value to write.
 */
template <typename T>
void write_csv_element(std::ofstream& file, const T& value);

/**
 * @brief Escapes a string value to be written to the CSV file.
 *
 * @param file The file stream to write to.
 * @param value The value to write.
 */
template <>
void write_csv_element<std::string>(std::ofstream& file, const std::string& value);
class CSVWriter {
   public:
    /**
     * @brief The types that can be written to the CSV file.
     */
    using serializable_types = std::variant<size_t, int, double, std::string>;

    /**
     * @brief Creates a new CSVWriter instance.
     *
     * @param file_path The path to the CSV file to write to.
     * @param headers The headers of the CSV file.
     * @param append Whether to append to the CSV file or overwrite it.
     * @param separator The separator to use between values.
     */
    CSVWriter(std::filesystem::path file_path, const std::vector<std::string>& headers, bool append = false, std::string separator = ";");

    /**
     * @brief Creates a new CSVWriter instance.
     *
     * @param file_path The path to the CSV file to write to.
     * @param append Whether to append to the CSV file or overwrite it.
     * @param separator The separator to use between values.
     */
    explicit CSVWriter(std::filesystem::path file_path, bool append = false, std::string separator = ";");

    // Delete copy constructor and assignment operator
    CSVWriter(const CSVWriter&) = delete;
    CSVWriter& operator=(CSVWriter&) = delete;

    // Move constructor and assignment operator
    CSVWriter(CSVWriter&&) noexcept;
    CSVWriter& operator=(CSVWriter&&) noexcept;

    ~CSVWriter();

    /**
     * @brief Initializes the CSV file and writes the header row.
     *
     * @param headers The headers of the CSV file.
     */
    void initialize(const std::vector<std::string>& headers);

    /**
     * @brief Writes a row to the CSV file.
     *
     * @param row The row to write.
     */
    void writeRow(const std::vector<serializable_types>& row);

   private:
    /**
     * @brief The file stream to write to.
     */
    std::ofstream file;

    /**
     * @brief The path to the CSV file to write to.
     */
    std::filesystem::path file_path;

    /**
     * @brief Whether to append to the CSV file or overwrite it.
     */
    bool append;

    /**
     * @brief The headers of the CSV file.
     */
    std::vector<std::string> headers;

    /**
     * @brief The separator to use between values.
     */
    std::string separator;
};