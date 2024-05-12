/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include <list>

#include "Initializer.h"

/** \brief
 *  Reads the initial data from a file
 *
 *  The file format is as follows:
 *  - The first line contains the number of particles
 *  - The following lines contain the position, velocity and mass of each
 * particle
 *  - The lines are formatted as follows:
 *    x1 x2 x3 v1 v2 v3 m
 *    where x1, x2, x3 are the position coordinates, v1, v2, v3 are the
 * velocity coordinates and m is the mass of the particle
 */
class FileReader : public Initializer {
   private:
    /** The name of the file to read from */
    const char* filename;

   public:
    /** \brief
     *  Constructor
     *
     *  \param filename_
     *  The name of the file to read from
     */
    explicit FileReader(const char* filename);

    /** \brief
     *  Destructor
     */
    ~FileReader() override = default;

    /** \brief
     *  Reads the data from the file and stores it in the given list
     *
     *  \param particles
     *  The list to store the particles in
     */
    void readData(std::list<Particle>& particles) override;
};
