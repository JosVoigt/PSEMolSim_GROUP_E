/*
 * XYZWriter.h
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 */

#pragma once

#include <fstream>
#include <vector>

#include "outputWriter/Writer.h"

namespace outputWriter {

class XYZWriter : public Writer {
   public:
    XYZWriter();

    ~XYZWriter();

    void plotParticles(std::shared_ptr<ParticleContainerInterface> &container,
                       const std::string &filename, int iteration) override;
    std::string typeString() override;
};

}  // namespace outputWriter
