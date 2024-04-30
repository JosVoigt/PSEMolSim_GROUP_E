/*
 * XYZWriter.h
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 */

#pragma once

#include <fstream>
#include <vector>

#include "Particle.h"
#include "writer.h"

namespace outputWriter {

class XYZWriter : writer {
   public:
    XYZWriter();

    ~XYZWriter();

    void plotParticles(std::vector<Particle> &particles,
                       const std::string &filename, int iteration) override;
};

}  // namespace outputWriter
