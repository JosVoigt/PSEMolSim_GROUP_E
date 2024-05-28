#include "LinkedCellContainer.h"

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
        static_cast<int>(particlePos[0] / getCellSize()),
        static_cast<int>(particlePos[1] / getCellSize()),
        static_cast<int>(particlePos[2] / getCellSize()),
    };

    return getAmountCellsX() * cellCoordinates[0] + getAmountCellsY() * cellCoordinates[1] + getAmountCellsZ() * cellCoordinates[2];
}


void LinkedCellContainer::insertParticle(const Particle& particle) {
    const int cellIndex = getCellFromParticle(particle.getX());
    cellVector[cellIndex].push_back(particle);
}

std::list<Particle> LinkedCellContainer::retrieveNeighbors(const std::array<double, 3>& particle) const {

    const int cellOfParticle = getCellFromParticle(particle);
    std::list<Particle> neighbours;

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







