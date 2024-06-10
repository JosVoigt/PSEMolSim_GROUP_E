#pragma once

#include "../utils/Parser.h"
#include "outputWriter/VTKWriter.h"

class XMLReader{
  const char* filename;

public:
  explicit XMLReader(const char* filename);

  ~XMLReader() = default;

  void readData(parser::options &opt) const;
};
