/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include <list>

#include "Initializer.h"

class FileReader : public Initializer {
   private:
    const char* filename;

   public:
    FileReader(const char* filename);
    ~FileReader() override = default;

    void readData(std::list<Particle>& particles) override;
};
