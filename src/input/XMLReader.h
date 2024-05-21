#include "input/Initializer.h"

class XMLReader : public Initializer {
public:
  XMLReader(std::string filename);

  void readData(std::list<Particle> &init) override;
};
