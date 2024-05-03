/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include <list>

#include "container/Particle.h"

class FileReader {
   public:
    FileReader() = default;

    void readFile(std::list<Particle>& particles, const char* filename);
};
