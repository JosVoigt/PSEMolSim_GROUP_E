#include "LinkedCellContainer.h"

#include <algorithm>
#include <set>


LinkedCellContainer::LinkedCellContainer(int const amountCellsX_, int const amountCellsY_, int const amountCellsZ_, double const r_c) :
r_c(r_c), amountCellsX(amountCellsX_), amountCellsY(amountCellsY_), amountCellsZ(amountCellsZ_) {
    cellSize = r_c * r_c;

    amountCellsX += 2;
    amountCellsY += 2;
    amountCellsZ += 2;

    containerSizeX = amountCellsX * r_c;
    containerSizeY = amountCellsY * r_c;
    containerSizeZ = amountCellsZ * r_c;

    cellVector.resize(amountCellsX * amountCellsY * amountCellsZ);
}

std::array<int, 3> LinkedCellContainer::getCellCoordinates(const Particle& particle) const {
    return {
        static_cast<int>(particle.getX()[0] / cellSize),
        static_cast<int>(particle.getX()[1] / cellSize),
        static_cast<int>(particle.getX()[2] / cellSize),
    };
}


int LinkedCellContainer::getIndexFromCoordinates(const std::array<int,3> cellCoordinates) const {
    return cellCoordinates[0] + amountCellsX * (cellCoordinates[1] + amountCellsY * cellCoordinates[2]);
}


void LinkedCellContainer::insertParticle(const Particle& particle) {
    std::array<int, 3> particleCellCoordinates = getCellCoordinates(particle);
    const int cellIndex = getIndexFromCoordinates(particleCellCoordinates);

    cellVector[cellIndex].push_back(particle);
}

bool LinkedCellContainer::findAndremoveOldParticle(const Particle& particle) const {
    std::array<int, 3> particleCellCoordinates = getCellCoordinates(particle);
    const int cellIndex = getIndexFromCoordinates(particleCellCoordinates);

    if (std::find(cellVector[cellIndex].begin(), cellVector[cellIndex].end(), particle) != cellVector[cellIndex].end()) {
        return false;
    }
    //The particle can only realistically move one cell in any direction in one frame at most, so we only need to check its neighbors
    std::vector<Particle> neighbors = retrieveNeighbors(particle);

    neighbors.erase(std::find(neighbors.begin(), neighbors.end(), particle), neighbors.end());
    return true;
}


std::vector<Particle> LinkedCellContainer::retrieveNeighbors(const Particle& particle) const {

    const std::array<int, 3> particleCellCoordinates = getCellCoordinates(particle);
    std::vector<Particle> neighbours;

    //Find bounds of neighbouring cells (this is needed so that cellOfParticle -1 / +1 doesn't go out of bounds)
    const int startX = particleCellCoordinates[0] == 1 ? 1 : particleCellCoordinates[0] - 1;
    const int endX = particleCellCoordinates[0] == amountCellsX - 2 ? amountCellsX - 2 : particleCellCoordinates[0] + 1;

    const int startY = particleCellCoordinates[1] == 1 ? 1 : particleCellCoordinates[1] - 1;
    const int endY = particleCellCoordinates[1] == amountCellsY - 2 ? amountCellsY - 2 : particleCellCoordinates[1] + 1;

    const int startZ = particleCellCoordinates[2] == 1 ? 1 : particleCellCoordinates[2] - 1;
    const int endZ = particleCellCoordinates[2] == amountCellsZ - 2 ? amountCellsZ - 2 : particleCellCoordinates[2] + 1;

    //Iterate over them and add all the cells into the list
    for (int x = startX; x <= endX; x++) {
        for (int y = startY; y <= endY; y++) {
            for (int z = startZ; z <= endZ; z++) {

                for (const Particle& neighbour : cellVector[x + (amountCellsX) * (y + (amountCellsY) * z)]) {

                    //Of course we wouldn't want to include the particle itself, otherwise it would mess up the force calculation
                    if (neighbour == particle) {
                        continue;
                    }
                    neighbours.push_back(neighbour);
                }
            }
        }
    }
    return neighbours;
}

std::vector<Particle> LinkedCellContainer::retrieveRelevantParticles(Particle& particle) {
    return retrieveNeighbors(particle);
}


std::vector<int> LinkedCellContainer::retrieveBoundaryCellIndices() const {

    //Hashmap is used to ensure that no duplicate cells are found (might cause bugs with ghost cells)
    std::set<int> indices;

    //For start and end of x
    for (int y = 1; y < amountCellsY - 1; y++) {
        for (int z = 1; z < amountCellsZ - 1; z++) {
            indices.insert(1 + amountCellsX * (y + amountCellsY * z));
            indices.insert((amountCellsX - 2) + amountCellsX * (y + amountCellsY * z));
        }
    }

    //For start and end of y
    for (int x = 1; x < amountCellsX - 1; x++) {
        for (int z = 1; z < amountCellsZ - 1; z++) {
            indices.insert(x + amountCellsX * (1 + amountCellsY * z));
            indices.insert(x + amountCellsX * ((amountCellsY-2) + amountCellsY * z));
        }
    }

    //For start and end of z
    for (int x = 1; x < amountCellsX - 1; x++) {
        for (int y = 1; y < amountCellsZ - 1; y++) {
            indices.insert(x + amountCellsX * (y + amountCellsZ));
            indices.insert( x + amountCellsX * (y + amountCellsY * (amountCellsZ - 2)));
        }
    }

    //Convert hash set to vector
    std::vector<int> indicesVector(indices.begin(), indices.end());
    return indicesVector;
}


std::vector<Particle> LinkedCellContainer::retrieveBoundaryParticles(const std::vector<int>& cellIndices) const {

    std::vector<Particle> boundaryParticles;

    for (const int index : cellIndices) {
        for (const Particle& particle : cellVector[index]) {
            boundaryParticles.push_back(particle);
        }
    }
    return boundaryParticles;
}

std::vector<int> LinkedCellContainer::retrieveHaloCellIndices() const {

    //Hashmap is used to ensure that no duplicate cells are found (might cause bugs with ghost cells)
    std::set<int> indices;

    //For start and end of x
    for (int y = 0; y < amountCellsY; y++) {
        for (int z = 0; z < amountCellsZ; z++) {
            indices.insert(amountCellsX * (y + amountCellsY * z));
            indices.insert((amountCellsX - 1) + amountCellsX * (y + amountCellsY * z));
        }
    }

    //For start and end of y
    for (int x = 0; x < amountCellsX; x++) {
        for (int z = 0; z < amountCellsZ; z++) {
            indices.insert(x + amountCellsX * (amountCellsY * z));
            indices.insert(x + amountCellsX * ((amountCellsY-1) + amountCellsY * z));
        }
    }

    //For start and end of z
    for (int x = 0; x < amountCellsX; x++) {
        for (int y = 0; y < amountCellsZ; y++) {
            indices.insert(x + amountCellsX * y);
            indices.insert( x + amountCellsX * (y + amountCellsY * (amountCellsZ - 1)));
        }
    }

    //Convert hash set to vector
    std::vector<int> indicesVector(indices.begin(), indices.end());
    return indicesVector;
}


std::vector<Particle> LinkedCellContainer::retrieveHaloParticles(const std::vector<int>& cellIndices) const {

    std::vector<Particle> haloParticles;

    for (const int index : cellIndices) {
        for (const Particle& particle : cellVector[index]) {
            haloParticles.push_back(particle);
        }
    }
    return haloParticles;
}


void LinkedCellContainer::deleteHaloParticles() {

    std::vector<int> haloCells = retrieveHaloCellIndices();

    for (const int index : haloCells) {
        cellVector[index].clear();
    }
}

std::vector <Particle> LinkedCellContainer::preprocessParticles() {

    std::vector<Particle> allParticles;

    for (const std::vector<Particle>& cell : cellVector) {
        for (const Particle& particle : cell) {
            allParticles.push_back(particle);
        }
    }
    return allParticles;
}

void LinkedCellContainer::updateParticles() {

    for (std::vector<Particle>& cell : cellVector) {
        for (Particle& particle : cell) {
            if (findAndremoveOldParticle(particle)) {
                insertParticle(particle);
            }
        }
        deleteHaloParticles();
    }
}

size_t LinkedCellContainer::size() const {

    size_t size = 0;

    for (const std::vector<Particle>& cell : cellVector) {
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

/**------------------------------------------ GETTERS ------------------------------------------**/


double LinkedCellContainer::getSizeX() const {
    return containerSizeX;
}

double LinkedCellContainer::getSizeY() const {
    return containerSizeY;
}

double LinkedCellContainer::getSizeZ() const {
    return containerSizeZ;
}

double LinkedCellContainer::getR_c() const {
    return r_c;
}

double LinkedCellContainer::getCellSize() const {
    return cellSize;
}

int LinkedCellContainer::getAmountCellsX() const {
    return amountCellsX;
}

int LinkedCellContainer::getAmountCellsY() const {
    return amountCellsY;
}

int LinkedCellContainer::getAmountCellsZ() const {
    return amountCellsZ;
}







