#include <gtest/gtest.h>

#include "utils/Parser.h"

TEST(ParseCuboidsTest, HandlesValidInput) {
    std::vector<CuboidGenerator> cuboids;
    std::string input = "[[1.0,2.0,3.0],[4.0,5.0,6.0],7.0,8.0,9,10,11,12.0]";
    parser::parseCuboids(input, cuboids);

    EXPECT_EQ(cuboids.size(), 1);
    CuboidGenerator cuboid = cuboids[0];

    EXPECT_EQ(cuboid.getInitialVelocity()[0], 1.0);
    EXPECT_EQ(cuboid.getInitialVelocity()[1], 2.0);
    EXPECT_EQ(cuboid.getInitialVelocity()[2], 3.0);

    EXPECT_EQ(cuboid.getLLFC()[0], 4.0);
    EXPECT_EQ(cuboid.getLLFC()[1], 5.0);
    EXPECT_EQ(cuboid.getLLFC()[2], 6.0);

    EXPECT_EQ(cuboid.getH(), 7.0);
    EXPECT_EQ(cuboid.getMass(), 8.0);
    EXPECT_EQ(cuboid.getX(), 9);
    EXPECT_EQ(cuboid.getY(), 10);
    EXPECT_EQ(cuboid.getZ(), 11);
    EXPECT_EQ(cuboid.getMeanBrownianMotion(), 12.0);
}

// TEST(ParseCuboidsTest, DeathTest_HandlesInvalidInput) {
//     std::vector<CuboidGenerator> cuboids;
//     std::string input = "[[1.0,2.0,3.0],[4.0,5.0,6.0],7.0,8.0,9,10,11]";
//     EXPECT_DEATH(parser::parseCuboids(input, cuboids), ".*");
// }
