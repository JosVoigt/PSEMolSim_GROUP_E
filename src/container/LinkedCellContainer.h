#pragma once

#include <list>
#include <vector>

#include "Particle.h"

/**
 * \brief
 * A particle container that uses the linked cell algorithm
 */
class LinkedCellContainer {
private:

    /**
     * The size of the container in every axis
     **/
    double containerSizeX, containerSizeY, containerSizeZ;

    /**
     * The cutoff point, which also determines the cell size
     */
    double r_c;

    double cellSize = r_c * r_c * r_c;

    /**
     * The amount of cells for each axis
     */
    int amountCellsX, amountCellsY, amountCellsZ;

    /**
     * The vector of cells and the particles each cell contains
     */
    std::vector<std::list<Particle>> cellVector;

    /**
     *\brief
     * Calculates the index of the cell, where the given particle is located
     * @param particlePos
     * The coordinates of the particle
     */
    [[nodiscard]] int getCellFromParticle(const std::array<double, 3>& particlePos) const;


    /**
     * \brief
     * Retrieves all particles that reside in the halo cells
     * @param cellIndices
     * The vector of indices of the halo cells
     */
    [[nodiscard]] std::vector<Particle> retrieveHaloParticles(const std::vector<int>& cellIndices) const;

public:
    /**
     * \brief
     * Constructs a container that uses the linked cell algorithm using the given container size and cutoff point
     * @param containerSizeX
     * The size of the container in the X axis
     * @param containerSizeY
     * The size of the container in the Y axis
     * @param containerSizeZ
     * The size of the container in the Z axis
     * @param r_c
     * The cutoff point
     */
    LinkedCellContainer(double containerSizeX, double containerSizeY, double containerSizeZ, double r_c);

    /**
     * \brief
     * Inserts a particle into the container
     * @param particle
     * The particle to be inserted in the container
     */
    void insertParticle(const Particle& particle);

    /**
     * \brief
     * Retrieves all the neighbors of the given particle using the given cutoff point
     * @param particle
     * The particle, the neighbors of which need to be retrieved
     */
    [[nodiscard]] std::vector<Particle> retrieveNeighbors(const std::array<double, 3>& particle) const;

    /**
     * \brief
     * Retrieve the indexes of boundary cells to save computation time
     */
    [[nodiscard]] std::vector<int> retrieveBoundaryCellIndices() const;

    /**
     * \brief
     * Retrieves all the particles residing in the boundary cells
     * @param cellIndices
     * The vector of boundary cell indices
     */
    [[nodiscard]] std::vector<Particle> retrieveBoundaryParticles(const std::vector<int>& cellIndices) const;


    /**
     * \brief
     * Retrieves the indices of all halo cells
     */
    [[nodiscard]] std::vector<int> retrieveHaloCellIndices() const;


    /**
     * \brief
     * Deletes all particles that reside in the halo cells
     */
    void deleteHaloParticles();


    //------------------------------------------ GETTERS ------------------------------------------

    /**
     * \brief
     * Returns the size of the container in the X axis
     */
    [[nodiscard]] double getSizeX() const;

    /**
    * \brief
    * Returns the size of the container in the Y axis
    */
    [[nodiscard]] double getSizeY() const;

    /**
    * \brief
    * Returns the size of the container in the Z axis
    */
    [[nodiscard]] double getSizeZ() const;

    /**
     * \brief
     * Returns the cutoff point
     */
    [[nodiscard]] double getR_c() const;

    /**
     * \brief
     * Returns the cell size
     */
    [[nodiscard]] double getCellSize() const;

    /**
     * \brief
     * Returns the amount of cells in the X axis (the container's width)
     */
    [[nodiscard]] int getAmountCellsX() const;

    /**
     * \brief
     * Returns the amount of cells in the Y axis (the container's depth)
     */
    [[nodiscard]] int getAmountCellsY() const;

    /**
     * \brief
     * Returns the amount of cells in the Z axis (the container's height)
     */
    [[nodiscard]] int getAmountCellsZ() const;
};




