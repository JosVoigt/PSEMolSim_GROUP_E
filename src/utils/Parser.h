#pragma once

#include <memory>
#include <string>
#include <vector>

#include "force/Force.h"
#include "input/geometricGenerators/CuboidGenerator.h"
#include "input/geometricGenerators/DiscGenerator.h"
#include "outputWriter/Writer.h"

namespace parser {

struct options {
  double delta_t{};
  double start{};
  double end{};
  int dimensions;
  int writeoutFrequency{};
  std::vector<std::string> filepath;
  std::vector<CuboidGenerator> cuboids;
    std::vector<DiscGenerator> discs;
  std::string outfile;
  std::shared_ptr<Writer> writer_;
  std::shared_ptr<Force> force_;
};

// predeclaration
void parseCuboids(std::string cuboid_s, std::vector<CuboidGenerator> &ret);

/**
 *\brief
 *  Parses the program options for the molecular simulation
 *  If tests should be executed all other arguments should be expected to be
 *  empty.
 *\param ac
 *  The amount of arguments (usually called argc) provided to the
 *  main function
 *\param av
 * The arguments (usually called argv) provided to the
 *  main function
 *\return
 *  an options struct containing the read options and settings
 */
options parse(int ac, char *av[]);

/**
 * \brief
 * Executes the DFA for the cuboid input string.
 * Input strings of the form
 * [[D,D,D],[D,D,D],D,D,I,I,I,D](,[[D,D,D],[D,D,D],D,D,I,I,I,D])*
 * are getting accepted.
 * D here means double, I integer.
 * It is assumed that Doubles and Integers only contain 0-9 '.' and '-'.
 * The square brackets are symbols, the round brackets belong to the REGEX.
 *
 * The meaning is the velocity (3Dvector) the lower left corner (3D vector) the
 * distance , the mass , x , y, z, and the mean of the brownian motion.
 * \param
 * cuboid_s The string to get parsed.
 * \return The list of CuboidGenerators that
 * have been extracted from the string
 */
std::vector<CuboidGenerator> &parseCuboids(std::string cuboid_s);

}  // namespace parser
