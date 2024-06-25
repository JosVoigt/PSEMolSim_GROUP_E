#include <gtest/gtest.h>

#include <array>
#include <vector>

#include "particles/containers/ParticleContainer.h"
#include "particles/containers/linkedcells/LinkedCellsContainer.h"

#define EXPECT_ARRAY_NEAR(a, b, tol)      \
    for (int i = 0; i < 3; i++) {         \
        EXPECT_NEAR((a)[i], (b)[i], tol); \
    };

/**
 * This test creates a container with a domain size of 1x1x1 and a cutoff radius of 1.0.
 * That results in a cell size of one and including the boundary cells, the container has 3x3x3 cells.
 * The cells should be indexed by z as an increase of one, y as an increase by (domain_num_cells[2] + 2) and x as an increase by
 * (domain_num_cells[1] + 2) * (domain_num_cells[2] + 2).
 * The cell coordinate origin is at the left lower front corner of the domain with (0, 0, 0). But all indices from -1 to
 * domain_num_cells[i] are valid as the ghost cells are added around the domain cells.
 */
TEST(LinkedCellsContainer, CellCoordToIndex1) {
    std::array<double, 3> domain_size = {1.0, 1.0, 1.0};
    double cutoff_radius = 1.0;

    LinkedCellsContainer container(domain_size, cutoff_radius);
    EXPECT_EQ(container.getCells().size(), 27);

    EXPECT_EQ(container.cellCoordToCellIndex(-2, 0, 0), -1);
    EXPECT_EQ(container.cellCoordToCellIndex(0, -2, 0), -1);
    EXPECT_EQ(container.cellCoordToCellIndex(0, 0, -2), -1);

    EXPECT_EQ(container.cellCoordToCellIndex(2, 0, 0), -1);
    EXPECT_EQ(container.cellCoordToCellIndex(0, 2, 0), -1);
    EXPECT_EQ(container.cellCoordToCellIndex(0, 0, 2), -1);

    EXPECT_EQ(container.cellCoordToCellIndex(-1, -1, -1), 0);
    EXPECT_EQ(container.cellCoordToCellIndex(1, 1, 1), 26);
    EXPECT_EQ(container.cellCoordToCellIndex(0, 0, 0), 13);

    EXPECT_EQ(container.cellCoordToCellIndex(-1, -1, 1), 2);
    EXPECT_EQ(container.cellCoordToCellIndex(-1, 0, -1), 3);
    EXPECT_EQ(container.cellCoordToCellIndex(-1, 0, 0), 4);
    EXPECT_EQ(container.cellCoordToCellIndex(1, -1, -1), 18);
    EXPECT_EQ(container.cellCoordToCellIndex(1, -1, 1), 20);
    EXPECT_EQ(container.cellCoordToCellIndex(1, 0, 0), 22);
}

/**
 * This test creates a container with a domain size of 1x2x3 and a cutoff radius of 1.0.
 * That results in a cell size of one and including the boundary cells, the container has 3x4x5 cells.
 * Tested is the method that converts cell coordinates to the corresponding index in the cells vector.
 */
TEST(LinkedCellsContainer, CellCoordToIndex2) {
    std::array<double, 3> domain_size = {1.0, 2.0, 3.0};
    double cutoff_radius = 1.0;

    LinkedCellsContainer container(domain_size, cutoff_radius);
    EXPECT_EQ(container.getCells().size(), 60);

    EXPECT_EQ(container.cellCoordToCellIndex(-2, 0, 0), -1);
    EXPECT_EQ(container.cellCoordToCellIndex(0, -2, 0), -1);
    EXPECT_EQ(container.cellCoordToCellIndex(0, 0, -2), -1);

    EXPECT_EQ(container.cellCoordToCellIndex(2, 0, 0), -1);
    EXPECT_EQ(container.cellCoordToCellIndex(0, 3, 0), -1);
    EXPECT_EQ(container.cellCoordToCellIndex(0, 0, 4), -1);

    EXPECT_EQ(container.cellCoordToCellIndex(-1, -1, -1), 0);
    EXPECT_EQ(container.cellCoordToCellIndex(1, 2, 3), 59);

    EXPECT_EQ(container.cellCoordToCellIndex(0, 0, 0), 26);
}

/**
 * This test creates a container with a domain size of 1x1x1 and a cutoff radius of 1.0.
 * That results in a cell size of one and including the boundary cells, the container has 3x3x3 cells.
 * Tested is the method that converts particle positions to a pointer to the containing cell in the cells vector.
 */
TEST(LinkedCellsContainer, ParticlePosToCell) {
    std::array<double, 3> domain_size = {1.0, 1.0, 1.0};
    double cutoff_radius = 1.0;

    LinkedCellsContainer container(domain_size, cutoff_radius);
    EXPECT_EQ(container.getCells().size(), 27);

    EXPECT_THROW(container.particlePosToCell(-1.1, 0, 0), std::runtime_error);
    EXPECT_THROW(container.particlePosToCell(0, -1.1, 0), std::runtime_error);
    EXPECT_THROW(container.particlePosToCell(0, 0, -1.1), std::runtime_error);

    EXPECT_THROW(container.particlePosToCell(2.1, 0, 0), std::runtime_error);
    EXPECT_THROW(container.particlePosToCell(0, 2.1, 0), std::runtime_error);
    EXPECT_THROW(container.particlePosToCell(0, 0, 2.1), std::runtime_error);

    EXPECT_EQ(container.particlePosToCell(-1.0, -1.0, -1.0), &container.getCells()[0]);
    EXPECT_EQ(container.particlePosToCell(1.0, 1.0, 1.0), &container.getCells()[26]);
    EXPECT_EQ(container.particlePosToCell(0.0, 0.0, 0.0), &container.getCells()[13]);

    EXPECT_EQ(container.particlePosToCell(-1.0, -1.0, 1.0), &container.getCells()[2]);
    EXPECT_EQ(container.particlePosToCell(-1.0, 0.0, -1.0), &container.getCells()[3]);
    EXPECT_EQ(container.particlePosToCell(-1.0, 0.0, 0.0), &container.getCells()[4]);
    EXPECT_EQ(container.particlePosToCell(1.0, -1.0, -1.0), &container.getCells()[18]);
    EXPECT_EQ(container.particlePosToCell(1.0, -1.0, 1.0), &container.getCells()[20]);
    EXPECT_EQ(container.particlePosToCell(1.0, 0.0, 0.0), &container.getCells()[22]);
}

/**
 * This test creates a container with a domain size of 1x1x1 and a cutoff radius of 1.0.
 * It tests the cell creation and the cell type assignment, by checking the cell type of each cell.
 * The expected values were calculated by hand.
 */
TEST(LinkedCellsContainer, CellCreationManual) {
    std::array<double, 3> domain_size = {1.0, 1.0, 1.0};
    double cutoff_radius = 1.0;

    LinkedCellsContainer container(domain_size, cutoff_radius);

    std::vector<Cell> cells = container.getCells();
    std::array<double, 3> cell_size = container.getCellSize();

    EXPECT_EQ(cells.size(), 3 * 3 * 3);
    EXPECT_NEAR(cell_size[0], 1.0, 1e-15);
    EXPECT_NEAR(cell_size[1], 1.0, 1e-15);
    EXPECT_NEAR(cell_size[2], 1.0, 1e-15);

    for (size_t i = 0; i < cells.size(); ++i) {
        if (i == 13) {
            EXPECT_EQ(cells[i].getCellType(), Cell::CellType::BOUNDARY);
        } else {
            EXPECT_EQ(cells[i].getCellType(), Cell::CellType::HALO);
        }
    }

    EXPECT_EQ(container.getBoundaryCells().size(), 1);
    EXPECT_EQ(container.getBoundaryCells()[0]->getCellType(), Cell::CellType::BOUNDARY);
}

/**
 * This test creates a container with a domain size of 1x1x1 and a cutoff radius of 0.1.
 * It tests the cell creation and the cell type assignment, by checking the cell type of each cell.
 * The expected values depend on the cell coordinates and thus require container.cellCoordToCellIndex() to be correct.
 */
TEST(LinkedCellsContainer, CellCreationCalculated) {
    std::array<double, 3> domain_size = {1.0, 1.0, 1.0};
    double cutoff_radius = 0.1;

    LinkedCellsContainer container(domain_size, cutoff_radius);

    std::vector<Cell> cells = container.getCells();
    std::array<double, 3> cell_size = container.getCellSize();
    std::array<int, 3> domain_num_cells = container.getDomainNumCells();

    EXPECT_NEAR(cell_size[0], 0.1, 1e-15);
    EXPECT_NEAR(cell_size[1], 0.1, 1e-15);
    EXPECT_NEAR(cell_size[2], 0.1, 1e-15);
    EXPECT_EQ(domain_num_cells[0], 10);
    EXPECT_EQ(domain_num_cells[1], 10);
    EXPECT_EQ(domain_num_cells[2], 10);
    EXPECT_EQ(cells.size(), (domain_num_cells[0] + 2) * (domain_num_cells[1] + 2) * (domain_num_cells[2] + 2));

    for (int cx = -1; cx < domain_num_cells[0] + 1; ++cx) {
        for (int cy = -1; cy < domain_num_cells[1] + 1; ++cy) {
            for (int cz = -1; cz < domain_num_cells[2] + 1; ++cz) {
                int index = container.cellCoordToCellIndex(cx, cy, cz);

                if (cx < 0 || cx >= domain_num_cells[0] || cy < 0 || cy >= domain_num_cells[1] || cz < 0 || cz >= domain_num_cells[2]) {
                    EXPECT_EQ(cells[index].getCellType(), Cell::CellType::HALO);
                } else if (cx == 0 || cx == domain_num_cells[0] - 1 || cy == 0 || cy == domain_num_cells[1] - 1 || cz == 0 ||
                           cz == domain_num_cells[2] - 1) {
                    EXPECT_EQ(cells[index].getCellType(), Cell::CellType::BOUNDARY);
                } else {
                    EXPECT_EQ(cells[index].getCellType(), Cell::CellType::INNER);
                }
            }
        }
    }
}

/**
 * This test creates a container with a domain size of 1x1x1 and a cutoff radius of 1.0
 * and inserts particles in the center of the domain, as well as the halo of the domain.
 * In addition, it tries to add particles outside of the domain, which should not be possible.
 */
TEST(LinkedCellsContainer, AddParticle) {
    std::array<double, 3> domain_size = {1.0, 1.0, 1.0};
    double cutoff_radius = 1.0;

    LinkedCellsContainer container(domain_size, cutoff_radius);

    Particle p1({0.5, 0.5, 0.5}, {0, 0, 0}, 0.0, 0.0);
    container.addParticle(p1);
    EXPECT_EQ(container.size(), 1);

    Particle p2({0.1, 0.1, 0.1}, {0, 0, 0}, 0.0, 0.0);
    container.addParticle(p2);
    EXPECT_EQ(container.size(), 2);

    // Particle h({-0.1, -0.1, -0.1}, {0, 0, 0}, 0.0, 0.0);
    // EXPECT_THROW(container.addParticle(h), std::runtime_error);

    Particle out_of_bounds({5, 5, 5}, {0, 0, 0}, 0.0, 0.0);
    EXPECT_THROW(container.addParticle(out_of_bounds), std::runtime_error);
}
