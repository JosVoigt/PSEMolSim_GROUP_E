#include "CSVWriter.h"

#include <utility>

#include "io/logger/Logger.h"

template <typename T>
void write_csv_element(std::ofstream& file, const T& value) {
    file << value;
}

template <>
void write_csv_element<std::string>(std::ofstream& file, const std::string& value) {
    file << "\"" << value << "\"";
}

CSVWriter::CSVWriter(std::filesystem::path file_path, const std::vector<std::string>& headers, bool append, std::string separator)
    : file_path(std::move(file_path)), append(append), headers(headers), separator(std::move(separator)) {
    initialize(headers);
}
CSVWriter::CSVWriter(std::filesystem::path file_path, bool append, std::string separator)
    : file_path(std::move(file_path)), append(append), separator(std::move(separator)) {}

CSVWriter::CSVWriter(CSVWriter&& rhs) noexcept {
    this->file_path = std::move(rhs.file_path);
    this->headers = std::move(rhs.headers);
    this->separator = std::move(rhs.separator);
    this->file = std::move(rhs.file);
}
CSVWriter& CSVWriter::operator=(CSVWriter&& rhs) noexcept {
    this->file_path = std::move(rhs.file_path);
    this->headers = std::move(rhs.headers);
    this->separator = std::move(rhs.separator);
    this->file = std::move(rhs.file);
    return *this;
}

CSVWriter::~CSVWriter() {
    Logger::logger->info("Closing CSVWriter for file {}...", file_path.string());
    file.close();
}

void CSVWriter::initialize(const std::vector<std::string>& headers) {
    Logger::logger->info("Creating CSVWriter for file {}...", file_path.string());

    this->headers = headers;

    if (!std::filesystem::exists(file_path)) {
        Logger::logger->info("File {} does not exist, creating it!", file_path.string());
        std::filesystem::create_directories(file_path.parent_path());
    } else {
        Logger::logger->warn("File {} already exists", file_path.string());
    }

    if (append) {
        Logger::logger->warn("Appending to file {}...", file_path.string());
        file.open(file_path, std::ios_base::app);
    } else {
        Logger::logger->warn("Overwriting file {}...", file_path.string());
        file.open(file_path, std::ios_base::out);
    }

    if (!file.is_open()) {
        Logger::logger->error("Could not open file {} for writing!", file_path.string());
        throw std::runtime_error("Could not open file for writing!");
    }

    writeRow({headers.begin(), headers.end()});
}

void CSVWriter::writeRow(const std::vector<CSVWriter::serializable_types>& row) {
    if (row.size() != headers.size()) {
        Logger::logger->error("Row size ({}) does not match cols size ({})!", row.size(), headers.size());
        throw std::runtime_error("Row size does not match cols size!");
    }

    for (size_t i = 0; i < row.size(); i++) {
        if (i > 0) {
            file << separator;
        }
        std::visit([&](auto&& arg) { write_csv_element(file, arg); }, row[i]);
    }
    file << std::endl;
}
