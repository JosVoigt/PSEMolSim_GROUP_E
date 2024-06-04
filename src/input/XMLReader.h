#pragma once

#include "input/Initializer.h"
#include "utils/Parser.h"

class XMLReader : public Initializer {
private:
  const char* filename;

public:
  explicit XMLReader(const char* filename);

  ~XMLReader() override = default;

  void readData(parser::options &opt);
};
