#pragma once

#include <memory>
#include <vector>

#include "Particle.h"
#include "LinkedCellBoundaryCondition/LinkedCellBoundary.h"
#include "ParticleContainerInterface.h"

/**
 * \brief
 * A particle container that uses the linked cell algorithm
 */
class LinkedCellContainer : public ParticleContainerInterface {
private:

    /**
     * The size of the container in every axis
     **/
    double containerSizeX, containerSizeY, containerSizeZ;

    /**
     * The cutoff point, which also determines the cell size
     */
    double r_c;

    double cellSize = r_c * r_c;

    /**
     * The amount of cells for each axis
     */
    int amountCellsX, amountCellsY, amountCellsZ;

    /**
     * The vector of cells and the particles in each cell it contains
     */
    std::vector<std::vector<Particle>> cellVector;

    /**
     * \brief
     * The boundary for each side of the container
     */
    std::vector<std::shared_ptr<LinkedCellBoundary>> cellBoundaries;


    /**
     * \brief
     * returns the coordinates of the cell that contains the given particle
     * @param particle
     */
    [[nodiscard]] std::array<int, 3> getCellCoordinates(const Particle& particle) const;

    /**
     *\brief
     * Calculates the index of the cell, where the given particle is located
     * @param cellCoordinates
     * The coordinates of the particle
     */
    [[nodiscard]] int getIndexFromCoordinates(std::array<int,3> cellCoordinates) const;


    /**
     * \brief
     * Retrieves the boundary cells that contain the particles according to the given cell indices
     * @param cellIndices
     * The vector of indices of the halo cells
     */
    [[nodiscard]] std::vector<std::vector<Particle>> &retrieveHaloParticles(const std::vector<int> &cellIndices) const;

public:
 /**
  *\brief
  * The boundary conditions for the cells
  */
	enum BoundaryConditions{
		outflow,
		reflection,
	 periodic
	};

 /**
  * \brief
  * The sides of the container
  */
 enum ContainerSide {
  west = 0,
  south = 1,
  east = 2,
  north = 3,
  up = 4,
  down = 5
};

    /**
     * \brief
     * Constructs a container that uses the linked cell algorithm using the given container size and cutoff point
     * @param amountCellsX_
     * The amount of cells on the X axis
     * @param amountCellsY_
     * The amount of cells on the Y axis
     * @param amountCellsZ_
     * The amount of cells on the Z axis
     * @param r_c
     * The cutoff point
     * @param conditions
     * An array of boundary conditions in the following order: West, South, East, North, Up, Down
     */
    LinkedCellContainer(int amountCellsX_, int amountCellsY_, int amountCellsZ_, double r_c, std::array<BoundaryConditions, 6> conditions);

    /**
     * \brief
     * Inserts a particle into the container
     * @param particle
     * The particle to be inserted in the container
     */
    void insertParticle(const Particle& particle);

    /**
     * \brief
     * Removes the particle only if it has changed cells and returns true, otherwise returns false
     * @param particle
     * The particle that is to be removed
     * @return
     */
    [[nodiscard]] bool findAndremoveOldParticle(const Particle& particle) const;



    /**
     * \brief
     * Retrieves all the neighbors of the given particle using the given cutoff point
     * @param particle
     * The particle, the neighbors of which need to be retrieved
     */
    [[nodiscard]] std::vector<Particle> retrieveNeighbors(const Particle& particle) const;

    /**
    * \brief
    * Retrieves the hallo cell indices of the given side of the container
    * @param side
    * The side of which the indices of the halo cells are to be retrieved
    */
    [[nodiscard]] std::vector<int> retrieveBoundaryCellIndices(ContainerSide side) const;

    /**
     * \brief
     * Retrieves all the boundary cells containing the particles according to the given cell indices
     * @param cellIndices
     * The vector of boundary cell indices
     */
    [[nodiscard]] std::vector<std::vector<Particle>> &retrieveBoundaryParticles(
     const std::vector<int> &cellIndices) const;


    /**
     * \brief
     * Retrieves the hallo cell indices of the given side of the container
     * @param side
     * The side of which the indices of the halo cells are to be retrieved
     */
    [[nodiscard]] std::vector<int> retrieveHaloCellIndices(ContainerSide side) const;


    /**
     * \brief
     * Retrieves the indices of all halo cells
     *
     */
    [[nodiscard]] std::vector<int> retrieveAllHaloCellIndices() const;

    /**
     * \brief
     * Deletes all particles that reside in the halo cells
     */
    void deleteHaloParticles();

    /**
     * \brief
     * Retrieves the relevant particles (in this case the particles that reside in the neighboring cells)
     * @param particle
     * The particle for which the relevant particles are to be retrieved
     */
    std::vector<Particle> retrieveRelevantParticles(Particle& particle) override;

    /**
     * \brief
     * Returns the list of all particles before the simulation starts
     */
    std::vector<Particle> preprocessParticles() override;

    /**
     * \brief
     * Updates all the particle positions
     */
    void updateParticles() override;

    /**
     * \brief
     * Returns the total amount of particles in the container
     */
    [[nodiscard]] std::size_t size() const override;

    /**
     * \brief
     * Returns an iterator to the beginning of the first cell in cellVector
     */
    std::vector<Particle>::iterator begin() override;

    /**
     * \brief
     * Returns an iterator to the end of the first cell in cellVector
     */
    std::vector<Particle>::iterator end() override;
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




