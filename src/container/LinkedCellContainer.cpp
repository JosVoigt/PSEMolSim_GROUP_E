#include "LinkedCellContainer.h"

#include <set>

LinkedCellContainer::LinkedCellContainer(double const containerSizeX, double const containerSizeY, double const containerSizeZ, double const r_c) :
containerSizeX(containerSizeX), containerSizeY(containerSizeY), containerSizeZ(containerSizeZ), r_c(r_c) {
    cellSize = r_c * r_c * r_c;
    amountCellsX = static_cast<int>(containerSizeX / cellSize);
    amountCellsY = static_cast<int>(containerSizeY / cellSize);
    amountCellsZ = static_cast<int>(containerSizeZ / cellSize);

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

    //Find bounds of neighbouring cells
    const int startX = cellOfParticle == 0 ? 0 : cellOfParticle - 1;
    const int endX = cellOfParticle == amountCellsX - 1 ? amountCellsX - 1 : cellOfParticle + 1;

    const int startY = cellOfParticle == 0 ? 0 : cellOfParticle - 1;
    const int endY = cellOfParticle == amountCellsY - 1 ? amountCellsY - 1 : cellOfParticle + 1;

    const int startZ = cellOfParticle == 0 ? 0 : cellOfParticle - 1;
    const int endZ = cellOfParticle == amountCellsZ - 1 ? amountCellsZ - 1 : cellOfParticle + 1;

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


std::vector<int> LinkedCellContainer::retrieveBoundaryCellIndexes() const {

    //Hashmap is used to ensure that no duplicate cells are found (might cause bugs with ghost cells)
    std::set<int> indices;

    //For start and end of x
    for (int y = 0; y <= amountCellsY; y++) {
        for (int z = 0; z <= amountCellsZ; z++) {
            indices.insert(amountCellsX * (y + amountCellsY * z));
            indices.insert((amountCellsX - 1) + amountCellsX * (y + amountCellsY * z));
        }
    }

    //For start and end of y
    for (int x = 0; x <= amountCellsX; x++) {
        for (int z = 0; z <= amountCellsZ; z++) {
            indices.insert(x + amountCellsX * (amountCellsY * z));
            indices.insert(x + amountCellsX * ((amountCellsY-1) + amountCellsY * z));
        }
    }

    //For start and end of z
    for (int x = 0; x <= amountCellsX; x++) {
        for (int y = 0; y <= amountCellsZ; y++) {
            indices.insert(x + amountCellsX * y);
            indices.insert( x + amountCellsX * (y + amountCellsY * (amountCellsZ - 1)));
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







