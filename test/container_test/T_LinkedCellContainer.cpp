#include <gtest/gtest.h>

#include <array>
#include <complex>

#include "container/LinkedCellContainer.h"
#include "container/Particle.h"

class LCC_test : public testing::Test {
 protected:
  std::array<double, 3> v1 = {0, 0, 0};
  std::array<double, 3> x1 = {4, 4, 4};

  std::array<double, 3> v2 = {0, 0, 0};
  std::array<double, 3> x2 = {0, 0, 0};

  std::array<double, 3> v3 = {0, 0, 0};
  std::array<double, 3> x3 = {8, 4, 4};

  Particle p1 = Particle(x1, v1, 1, 0);
  Particle p2 = Particle(x2, v2, 1, 0);
  Particle p3 = Particle(x3, v3, 1, 0);

  LinkedCellContainer container = LinkedCellContainer(
      10, 10, 10, 2,
      std::array<LinkedCellContainer::BoundaryConditions, 6>(
          {LinkedCellContainer::outflow, LinkedCellContainer::outflow,
           LinkedCellContainer::outflow, LinkedCellContainer::outflow,
           LinkedCellContainer::outflow, LinkedCellContainer::outflow}));
};

TEST_F(LCC_test, insertParticle) {
  container.insertParticle(p1);
  std::vector<Particle> particles = container.preprocessParticles();

  EXPECT_EQ(particles, std::vector({p1}));
}

TEST_F(LCC_test, findAndremoveOldParticle) {
  container.insertParticle(p1);
  EXPECT_FALSE(container.findAndremoveOldParticle(p1));
}

TEST_F(LCC_test, cellInformation) {
  EXPECT_EQ(container.getCellSize(), 4);
  EXPECT_EQ(container.getAmountCellsX() * container.getAmountCellsY() *
                container.getAmountCellsZ(),
            std::pow(12, 3));
  EXPECT_EQ(container.getSizeX(), 24);
}

TEST_F(LCC_test, updateParticles) {
  container.insertParticle(p2);
  container.updateParticles();

  EXPECT_TRUE(container.preprocessParticles().empty());
}

TEST_F(LCC_test, retrieveBoundaryParticles) {
  container.insertParticle(p1);

  auto indices = container.retrieveBoundaryCellIndices(LinkedCellContainer::up);
  const std::vector<std::vector<Particle>> boundaryParticles =
      container.retrieveBoundaryParticles(indices);

  std::vector<std::vector<Particle>> expected = {{p1}};

  EXPECT_EQ(boundaryParticles, expected);
}

TEST_F(LCC_test, retrieveNeighbors) {
  container.insertParticle(p1);
  container.insertParticle(p3);

  auto neighbors = container.retrieveRelevantParticles(p1);

  EXPECT_EQ(neighbors, std::vector({p3}));

  neighbors = container.retrieveRelevantParticles(p3);

  EXPECT_EQ(neighbors, std::vector({p1}));
}
