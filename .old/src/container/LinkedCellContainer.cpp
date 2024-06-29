#include "LinkedCellContainer.h"

#include <algorithm>
#include <set>

#include "LinkedCellBoundaryCondition/BoundCondReflection.h"
#include "force/PlanetForce.h"

LinkedCellContainer::LinkedCellContainer(
    int const amountCellsX_, int const amountCellsY_, int const amountCellsZ_,
    double const r_c, std::array<BoundaryConditions, 6> conditions, std::list<Particle> &init)
    : amountCellsX(amountCellsX_),
      amountCellsY(amountCellsY_),
      amountCellsZ(amountCellsZ_),
      r_c(r_c),
      ParticleContainerInterface(init) {
  cellSize = r_c * r_c * r_c;

  amountCellsX += 2;
  amountCellsY += 2;
  amountCellsZ += 2;

  containerSizeX = amountCellsX * r_c;
  containerSizeY = amountCellsY * r_c;
  containerSizeZ = amountCellsZ * r_c;

  cellVector.resize(amountCellsX * amountCellsY * amountCellsZ);

  // plane vector constants W,S,E,N,U,D
  std::array<std::array<double, 3>, 6> plane_vectors = {
      {{0, 0, -1}, {-1, 0, 0}, {0, 0, 1}, {1, 0, 0}, {0, 1, 0}, {0, -1, 0}}};
  std::array<std::array<double, 3>, 6> point_vectors = {
      {{0, 0, 0},
       {0, 0, 0},
       {containerSizeX, containerSizeY, containerSizeZ},
       {containerSizeX, containerSizeY, containerSizeZ},
       {containerSizeX, containerSizeY, containerSizeZ},
       {0, 0, 0}}};

  std::vector<int> currentBoundaryIndices;
  std::vector<int> currentHaloIndices;

  for (int i = 0; i < 6; i++) {

    std::vector<std::vector<Particle>> boundaryCells = {};
    std::vector<std::vector<Particle>> haloCells = {};

    if (conditions[i] == outflow) {
      currentHaloIndices =
          retrieveHaloCellIndices(static_cast<ContainerSide>(i));

      cellBoundaries.push_back(std::make_shared<BoundaryConditionOutflow>(
          retrieveHaloParticles(currentHaloIndices, haloCells)));
    }
     if (conditions[i] == reflection) {
      currentBoundaryIndices =
          retrieveBoundaryCellIndices(static_cast<ContainerSide>(i));

      cellBoundaries.push_back(std::make_shared<BoundaryConditionReflection>(
          retrieveBoundaryParticles(currentBoundaryIndices, boundaryCells),
          std::make_shared<PlanetForce>(), plane_vectors[i], point_vectors[i]));
    }
    // add periodic when done
  }
}

std::array<int, 3> LinkedCellContainer::getCellCoordinates(
    const Particle &particle) const {
  return {
      static_cast<int>(particle.getX()[0] / cellSize),
      static_cast<int>(particle.getX()[1] / cellSize),
      static_cast<int>(particle.getX()[2] / cellSize),
  };
}

int LinkedCellContainer::getIndexFromCoordinates(
    const std::array<int, 3> cellCoordinates) const {
  return cellCoordinates[0] +
         amountCellsX *
             (cellCoordinates[1] + amountCellsY * cellCoordinates[2]);
}

void LinkedCellContainer::insertParticle(const Particle &particle) {
  std::array<int, 3> particleCellCoordinates = getCellCoordinates(particle);
  const int cellIndex = getIndexFromCoordinates(particleCellCoordinates);

  cellVector[cellIndex].push_back(particle);
}

bool LinkedCellContainer::findAndremoveOldParticle(
    const Particle &particle) const {
  std::array<int, 3> particleCellCoordinates = getCellCoordinates(particle);
  const int cellIndex = getIndexFromCoordinates(particleCellCoordinates);

  if (std::find(cellVector[cellIndex].begin(), cellVector[cellIndex].end(),
                particle) != cellVector[cellIndex].end()) {
    return false;
  }
  // The particle can only realistically move one cell in any direction in one
  // frame at most, so we only need to check its neighbors
  std::vector<Particle> neighbors = retrieveNeighbors(particle);

  neighbors.erase(std::find(neighbors.begin(), neighbors.end(), particle),
                  neighbors.end());
  return true;
}

std::vector<Particle> LinkedCellContainer::retrieveNeighbors(
    const Particle &particle) const {
  const std::array<int, 3> particleCellCoordinates =
      getCellCoordinates(particle);
  std::vector<Particle> neighbours;
  //To check for duplicate indices in edge cases
  std::vector<int> insertedIndices;

  // Find bounds of neighbouring cells (this is needed so that cellOfParticle -1
  // / +1 doesn't go out of bounds)
  //const int startX =
  //    particleCellCoordinates[0] == 1 ? 1 : particleCellCoordinates[0] - 1;
  const int endX = particleCellCoordinates[0] == amountCellsX - 2
                       ? amountCellsX - 2
                       : particleCellCoordinates[0] + 1;

  const int startY =
      particleCellCoordinates[1] == 1 ? 1 : particleCellCoordinates[1] - 1;
  const int endY = particleCellCoordinates[1] == amountCellsY - 2
                       ? amountCellsY - 2
                       : particleCellCoordinates[1] + 1;

  const int startZ =
      particleCellCoordinates[2] == 1 ? 1 : particleCellCoordinates[2] - 1;
  const int endZ = particleCellCoordinates[2] == amountCellsZ - 2
                       ? amountCellsZ - 2
                       : particleCellCoordinates[2] + 1;


  //Only including certain parts of the neighbours so that cells do not get counted twice, which messes up with Newton's third law
  //index = x * (lengthY * lengthZ) + y * lengthZ + z

  //East side of the neighbours
  for (int y = startY; y <= endY; y++) {
    for (int z = startZ; z <= endZ; z++) {

      int currentIndex = endX * (amountCellsY * amountCellsZ) + y * amountCellsZ + z;

      if (std::find(insertedIndices.begin(), insertedIndices.end(), currentIndex) != insertedIndices.end()) {
        continue;
      }

      for (const Particle &neighbour : cellVector[currentIndex]) {
        neighbours.push_back(neighbour);
        insertedIndices.push_back(currentIndex);
      }
    }
  }

  //Middle stripe along the Y axis in south side of neighbours
  for (int z = startZ; z <= endZ; z++) {

    int currentIndex = particleCellCoordinates[0] * (amountCellsY * amountCellsZ) + startY * amountCellsZ + z;

    if (std::find(insertedIndices.begin(), insertedIndices.end(), currentIndex) != insertedIndices.end()) {
      continue;
    }
    for (const Particle &neighbour : cellVector[currentIndex]) {

      if (neighbour == particle) {
        continue;
      }

      neighbours.push_back(neighbour);
      insertedIndices.push_back(currentIndex);
    }
  }

  //Cell neighbour, directly under the particle's cell

  int currentIndex = particleCellCoordinates[0] * (amountCellsY * amountCellsZ) + particleCellCoordinates[1] * amountCellsZ + startZ;

  if (std::find(insertedIndices.begin(), insertedIndices.end(), currentIndex) == insertedIndices.end()) {
    for (const Particle &neighbour : cellVector[currentIndex]) {

      if (neighbour == particle) {
        continue;
      }

      neighbours.push_back(neighbour);
      insertedIndices.push_back(currentIndex);
    }
  }

  //The particle's cell itself
  currentIndex = particleCellCoordinates[0] * (amountCellsY * amountCellsZ) + particleCellCoordinates[1] * amountCellsZ + particleCellCoordinates[2];

  if (std::find(insertedIndices.begin(), insertedIndices.end(), currentIndex) == insertedIndices.end()) {
    for (const Particle &neighbour : cellVector[currentIndex]) {

      // Of course we wouldn't want to include the particle itself,
      //otherwise it would mess up the force calculation
      if (neighbour == particle) {
        continue;
      }
      neighbours.push_back(neighbour);
      insertedIndices.push_back(currentIndex);
    }
  }

  return neighbours;
}

std::vector<Particle> LinkedCellContainer::retrieveRelevantParticles(
    Particle &particle) {
  return retrieveNeighbors(particle);
}

std::vector<int> LinkedCellContainer::retrieveBoundaryCellIndices(
    const ContainerSide side) const {
  std::vector<int> indices;

  //index = x * (lengthY * lengthZ) + y * lengthZ + z

  if (side == south) {
    for (int x = 1; x < amountCellsX - 1; x++) {
      for (int z = 1; z < amountCellsZ - 1; z++) {
        indices.push_back(x * (amountCellsY * amountCellsZ) + 1 * amountCellsZ + z);
      }
    }
    return indices;
  }
  if (side == north) {
    for (int x = 1; x < amountCellsX - 1; x++) {
      for (int z = 1; z < amountCellsZ - 1; z++) {
        indices.push_back(x * (amountCellsY * amountCellsZ) + (amountCellsY - 2) * amountCellsZ + z);
      }
    }
    return indices;
  }
  if (side == west) {
    for (int y = 1; y < amountCellsY - 1; y++) {
      for (int z = 1; z < amountCellsZ - 1; z++) {
        indices.push_back(1 * (amountCellsY * amountCellsZ) + y * amountCellsZ + z);
      }
    }
    return indices;
  }
  if (side == east) {
    for (int y = 1; y < amountCellsY - 1; y++) {
      for (int z = 1; z < amountCellsZ - 1; z++) {
        indices.push_back((amountCellsX - 2) * (amountCellsY * amountCellsZ) - 1 - y * amountCellsZ - z);
      }
    }
    return indices;
  }
  if (side == up) {
    for (int x = 1; x < amountCellsX - 1; x++) {
      for (int y = 1; y < amountCellsY - 1; y++) {
        indices.push_back(x * (amountCellsY * amountCellsZ) + y * amountCellsZ + (amountCellsZ - 2));
      }
    }
    return indices;
  }
  // if (side == down)
  for (int x = 1; x < amountCellsX - 1; x++) {
    for (int y = 1; y < amountCellsY - 1; y++) {
      indices.push_back(x * (amountCellsY * amountCellsZ) + y * amountCellsZ + 1);
    }
  }
  return indices;
}

std::vector<std::vector<Particle>>
    &LinkedCellContainer::retrieveBoundaryParticles(
        const std::vector<int> &cellIndices, std::vector<std::vector<Particle>>& boundaryCells) const {

  for (const int index : cellIndices) {
    boundaryCells.push_back(cellVector[index]);
  }
  return boundaryCells;
}

std::vector<int> LinkedCellContainer::retrieveHaloCellIndices(
    const ContainerSide side) const {
  std::vector<int> indices;


  //index = x * (lengthY * lengthZ) + y * lengthZ + z

  if (side == south) {
    for (int x = 0; x < amountCellsX; x++) {
      for (int z = 0; z < amountCellsZ; z++) {
        indices.push_back(x * (amountCellsY * amountCellsZ) + 0 * amountCellsZ + z);
      }
      indices.back() -= 1;
    }
    return indices;
  }
  if (side == north) {
    for (int x = 0; x < amountCellsX; x++) {
      for (int z = 0; z < amountCellsZ; z++) {
        indices.push_back(x * (amountCellsY * amountCellsZ) + (amountCellsY-1) * amountCellsZ + z);
      }
    }
    return indices;
  }
  if (side == west) {
    for (int y = 0; y < amountCellsY; y++) {
      for (int z = 0; z < amountCellsZ; z++) {
        indices.push_back(0 * (amountCellsY * amountCellsZ) + y * amountCellsZ + z);
      }
    }
    return indices;
  }
  if (side == east) {
    for (int y = 0; y < amountCellsY; y++) {
      for (int z = 0; z < amountCellsZ; z++) {
        indices.push_back(amountCellsX * (amountCellsY * amountCellsZ) - 1 - y * amountCellsZ - z);
      }
    }
    return indices;
  }
  if (side == up) {
    for (int x = 0; x < amountCellsX; x++) {
      for (int y = 0; y < amountCellsY; y++) {
        indices.push_back(x * (amountCellsY * amountCellsZ) + y * amountCellsZ + (amountCellsZ-1));
      }
    }
    return indices;
  }
  // if (side == down)
  for (int x = 0; x < amountCellsX; x++) {
    for (int y = 0; y < amountCellsY; y++) {
      indices.push_back(x * (amountCellsY * amountCellsZ) + y * amountCellsZ + 0);
    }
  }
  return indices;
}

std::vector<std::vector<Particle>> &LinkedCellContainer::retrieveHaloParticles(
    const std::vector<int> &cellIndices, std::vector<std::vector<Particle>>& haloCells) const {

  for (const int index : cellIndices) {
    haloCells.push_back(cellVector[index]);
  }
  return haloCells;
}

std::vector<int> LinkedCellContainer::retrieveAllHaloCellIndices() const {
  std::vector<int> allIndices = retrieveHaloCellIndices(south);

  // We start from 1 because we already have the front side
  for (int i = 1; i < 5; i++) {
    std::vector<int> currentSideIndices =
        retrieveHaloCellIndices(static_cast<ContainerSide>(i));
    allIndices.insert(allIndices.end(), currentSideIndices.begin(),
                      currentSideIndices.end());
  }
  return allIndices;
}

void LinkedCellContainer::deleteHaloParticles() {
  std::vector<int> haloCells = retrieveAllHaloCellIndices();

  for (const int index : haloCells) {
    cellVector[index].clear();
  }
}

std::vector<Particle> LinkedCellContainer::preprocessParticles() {
  std::vector<Particle> allParticles;

  for (const std::vector<Particle> &cell : cellVector) {
    for (const Particle &particle : cell) {
      allParticles.push_back(particle);
    }
  }
  return allParticles;
}

void LinkedCellContainer::updateParticles() {
  for (const auto &boundary : cellBoundaries) {
    boundary->executeBoundaryCondition();
  }

  for (std::vector<Particle> &cell : cellVector) {
    for (Particle &particle : cell) {
      if (findAndremoveOldParticle(particle)) {
        insertParticle(particle);
      }
    }
    deleteHaloParticles();
  }
}

size_t LinkedCellContainer::size() const {
  size_t size = 0;

  for (const std::vector<Particle> &cell : cellVector) {
    size += cell.size();
  }
  return size;
}

std::vector<Particle>::iterator LinkedCellContainer::begin() {
  return cellVector[0].begin();
}

std::vector<Particle>::iterator LinkedCellContainer::end() {
  return cellVector[0].end();
}

/**------------------------------------------ GETTERS
 * ------------------------------------------**/

double LinkedCellContainer::getSizeX() const { return containerSizeX; }

double LinkedCellContainer::getSizeY() const { return containerSizeY; }

double LinkedCellContainer::getSizeZ() const { return containerSizeZ; }

double LinkedCellContainer::getR_c() const { return r_c; }

double LinkedCellContainer::getCellSize() const { return cellSize; }

int LinkedCellContainer::getAmountCellsX() const { return amountCellsX; }

int LinkedCellContainer::getAmountCellsY() const { return amountCellsY; }

int LinkedCellContainer::getAmountCellsZ() const { return amountCellsZ; }