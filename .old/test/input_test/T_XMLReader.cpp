#include <gtest/gtest.h>

#include <fstream>

#include "input/XMLReader.h"

TEST(XMLReaderTest, ReadDataTest) {
  const char* testFileName = "testFile.txt";
  std::ofstream testFile(testFileName);
  testFile << "Velocity: X: 0, Y: 0, Z: 0\n";
  testFile << "LowerLeftCorner: X: 0.6, Y: 2, Z: 0\n";
  testFile << "Distance: 1.2\n";
  testFile << "Mass: 1\n";
  testFile << "X: 50\n";
  testFile << "Y: 14\n";
  testFile << "Z: 1\n";
  testFile << "BrownianMotionMean: 0\n";
  testFile << "Type_Cuboid: 1\n";
  testFile << "Velocity: X: 0, Y: 0, Z: 0\n";
  testFile << "LowerLeftCorner: X: 0.6, Y: 19, Z: 0\n";
  testFile << "Distance: 1.2\n";
  testFile << "Mass: 2\n";
  testFile << "X: 50\n";
  testFile << "Y: 14\n";
  testFile << "Z: 1\n";
  testFile << "BrownianMotionMean: 0\n";
  testFile << "Type_Cuboid: 2\n";
  testFile << "Epsilon: 1\n";
  testFile << "Sigma: 1\n";
  testFile << "Type: 1\n";
  testFile << "Epsilon: 1\n";
  testFile << "Sigma: 0.9412\n";
  testFile << "Type: 2\n";
  testFile << "AmountCellsX: 63\n";
  testFile << "AmountCellsY: 36\n";
  testFile << "AmountCellsZ: 1\n";
  testFile << "Sidelength: 2.5\n";
  testFile << "MaxChangeRate: 0.1\n";
  testFile << "Dimensions_Thermostat: 2\n";
  testFile << "Delta: 0.0005\n";
  testFile << "Frequency: 10\n";
  testFile << "Dimensions: 2";
  testFile << "Start: 0\n";
  testFile << "End: 25\n";
  testFile << "Outfile: small_experiment\n";
  testFile.close();

  XMLReader xml_reader(testFileName);

  parser::options options;
  xml_reader.readData(options);

  EXPECT_EQ(options.cuboids[0].getInitialVelocity()[0], 0.0);
  EXPECT_EQ(options.cuboids[0].getInitialVelocity()[1], 0.0);
  EXPECT_EQ(options.cuboids[0].getInitialVelocity()[2], 0.0);
  EXPECT_EQ(options.cuboids[0].getLLFC()[0], 0.6);
  EXPECT_EQ(options.cuboids[0].getLLFC()[1], 2.0);
  EXPECT_EQ(options.cuboids[0].getLLFC()[2], 0.0);
  EXPECT_EQ(options.cuboids[0].getH(), 1.2);
  EXPECT_EQ(options.cuboids[0].getMass(), 1.0);
  EXPECT_EQ(options.cuboids[0].getX(), 50);
  EXPECT_EQ(options.cuboids[0].getY(), 14);
  EXPECT_EQ(options.cuboids[0].getZ(), 1);
  EXPECT_EQ(options.cuboids[0].getMeanBrownianMotion(), 0.0);
  EXPECT_EQ(options.cuboids[0].getType(), 1);

  EXPECT_EQ(options.cuboids[1].getInitialVelocity()[0], 0.0);
  EXPECT_EQ(options.cuboids[1].getInitialVelocity()[1], 0.0);
  EXPECT_EQ(options.cuboids[1].getInitialVelocity()[2], 0.0);
  EXPECT_EQ(options.cuboids[1].getLLFC()[0], 0.6);
  EXPECT_EQ(options.cuboids[1].getLLFC()[1], 19);
  EXPECT_EQ(options.cuboids[1].getLLFC()[2], 0.0);
  EXPECT_EQ(options.cuboids[1].getH(), 1.2);
  EXPECT_EQ(options.cuboids[1].getMass(), 2.0);
  EXPECT_EQ(options.cuboids[1].getX(), 50);
  EXPECT_EQ(options.cuboids[1].getY(), 14);
  EXPECT_EQ(options.cuboids[1].getZ(), 1);
  EXPECT_EQ(options.cuboids[1].getMeanBrownianMotion(), 0);
  EXPECT_EQ(options.cuboids[1].getType(), 2);

  // epsilon
  // sigma
  EXPECT_EQ(options.delta_t, 0.0005);
  EXPECT_EQ(options.writeoutFrequency, 10);
  EXPECT_EQ(options.start, 0);
  EXPECT_EQ(options.end, 25);
  EXPECT_EQ(options.outfile, "small_experiment");
}