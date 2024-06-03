#include "LinkedCellContainer.h"

#include <set>

LinkedCellContainer::LinkedCellContainer(int const amountCellsX_, int const amountCellsY_, int const amountCellsZ_, double const r_c) :
amountCellsX(amountCellsX_), amountCellsY(amountCellsY_), amountCellsZ(amountCellsZ_), r_c(r_c) {
    cellSize = r_c * r_c * r_c;

    amountCellsX += 2;
    amountCellsY += 2;
    amountCellsZ += 2;

    containerSizeX = amountCellsX * r_c;
    containerSizeY = amountCellsY * r_c;
    containerSizeZ = amountCellsZ * r_c;

    cellVector.resize(amountCellsX * amountCellsY * amountCellsZ);
}

int LinkedCellContainer::getCellFromParticle(const std::array<double, 3>& particlePos) const {

    static std::array<int, 3> cellCoordinates = {
        static_cast<int>(particlePos[0] / cellSize),
        static_cast<int>(particlePos[1] / cellSize),
        static_cast<int>(particlePos[2] / cellSize),
    };

    return cellCoordinates[0] + amountCellsX * (cellCoordinates[1] + amountCellsY * cellCoordinates[2]);
}


void LinkedCellContainer::insertParticle(const Particle& particle) {
    const int cellIndex = getCellFromParticle(particle.getX());
    cellVector[cellIndex].push_back(particle);
}

std::vector<Particle> LinkedCellContainer::retrieveNeighbors(const std::array<double, 3>& particle) const {

    const int cellOfParticle = getCellFromParticle(particle);
    std::vector<Particle> neighbours;

    //Find bounds of neighbouring cells (this is needed so that cellOfParticle -1 / +1 doesn't go out of bounds)
    const int startX = cellOfParticle == 1 ? 1 : cellOfParticle - 1;
    const int endX = cellOfParticle == amountCellsX - 2 ? amountCellsX - 2 : cellOfParticle + 1;

    const int startY = cellOfParticle == 1 ? 1 : cellOfParticle - 1;
    const int endY = cellOfParticle == amountCellsY - 2 ? amountCellsY - 2 : cellOfParticle + 1;

    const int startZ = cellOfParticle == 1 ? 1 : cellOfParticle - 1;
    const int endZ = cellOfParticle == amountCellsZ - 2 ? amountCellsZ - 2 : cellOfParticle + 1;

    //Iterate over them and add all the cells into the list
    for (int x = startX; x <= endX; x++) {
        for (int y = startY; y <= endY; y++) {
            for (int z = startZ; z <= endZ; z++) {
                for (const Particle& neighbour : cellVector[x + amountCellsX * (y + amountCellsY * z)]) {
                    neighbours.push_back(neighbour);
                }
            }
        }
    }
    return neighbours;
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
    std::vector indicesVector(indices.begin(), indices.end());
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
    std::vector indicesVector(indices.begin(), indices.end());
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







