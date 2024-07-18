#include <io/logger/Logger.h>

#include <filesystem>
#include <string>

#pragma once

class FileLoader {
   public:
    /**
     * @brief Gets the path to the data directory
     *
     * Uses the cmake target_compile_definitions PRIVATE TEST_DATA_DIR to define the path to the data directory.
     */
    static std::string get_test_data_dir() {
#ifdef TEST_DATA_DIR
        return TEST_DATA_DIR;
#else
        Logger::logger->error("Error: TEST_DATA_DIR not defined.");
#endif
    }

    /**
     * @brief Gets the path to a file in the input directory
     *
     * @param file_name The name of the file
     * @return std::filesystem::path The path to the file
     */
    static std::filesystem::path get_input_file_path(const std::string& file_name) { return {get_test_data_dir() + "/input/" + file_name}; }
};
