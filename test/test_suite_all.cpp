#include <gtest/gtest.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

int main(int argc, char* argv[]) {
  auto console_logger = spdlog::stderr_color_mt("console");
  try {
    std::stringstream ss;
    std::string time_format = "%Y-%m-%d_%H-%M-%S";

    auto start = std::chrono::system_clock::now();
    time_t start_time = std::chrono::system_clock::to_time_t(start);

    ss << "logs/MolSim_"
       << std::put_time(std::localtime(&start_time), time_format.c_str())
       << ".log";

    auto file_logger = spdlog::basic_logger_mt("file", ss.str());
  } catch (const spdlog::spdlog_ex& ex) {
    spdlog::get("console")->critical("File logger could not be initalizied: {}",
                                     ex.what());
    exit(1);
  }
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
