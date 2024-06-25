#pragma once

#include <filesystem>

struct CLIParams {
    /**
     * Path to the input file
     */
    std::filesystem::path input_file_path;

    /**
     * Whether to run a performance test
     */
    bool performance_test;

    /**
     * Whether to use cached data or rerun all the (sub)simulations from scratch
     */
    bool fresh;
};