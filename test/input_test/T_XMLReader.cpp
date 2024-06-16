#include <gtest/gtest.h>

#include <fstream>

#include "input/XMLReader.h"

TEST(XMLReaderTest, ReadDataTest) {
  const char* testFileName = "testFile.txt";
  std::ofstream testFile(testFileName);
  testFile << "Velocity: X: 0, Y: 0, Z: 0\n";
  testFile << "LowerLeftCorner: X: 0, Y: 0, Z: 0\n";
  testFile << "Distance: 1.1225\n";
  testFile << "Mass: 1\n";
  testFile << "X: 40\n";
  testFile << "Y: 8\n";
  testFile << "Z: 1\n";
  testFile << "BrownianMotionMean: 0.1\n";
  testFile << "Velocity: X: 0, Y: -10, Z: 0\n";
  testFile << "LowerLeftCorner: X: 15, Y: 15, Z: 0\n";
  testFile << "Distance: 1.1225\n";
  testFile << "Mass: 1\n";
  testFile << "X: 8\n";
  testFile << "Y: 8\n";
  testFile << "Z: 1\n";
  testFile << "BrownianMotionMean: 0.1\n";
  testFile << "Epsilon: 5\n";
  testFile << "Sigma: 1\n";
  testFile << "Delta: 0.0002\n";
  testFile << "Frequency: 10\n";
  testFile << "Start: 0\n";
  testFile << "End: 5\n";
  testFile << "Outfile: simulation\n";
  testFile.close();

  XMLReader xml_reader(testFileName);

  parser::options options;
  xml_reader.readData(options);

  EXPECT_EQ(options.cuboids[0].getInitialVelocity()[0], 0.0);
  EXPECT_EQ(options.cuboids[0].getInitialVelocity()[1], 0.0);
  EXPECT_EQ(options.cuboids[0].getInitialVelocity()[2], 0.0);
  EXPECT_EQ(options.cuboids[0].getLLFC()[0], 0.0);
  EXPECT_EQ(options.cuboids[0].getLLFC()[1], 0.0);
  EXPECT_EQ(options.cuboids[0].getLLFC()[2], 0.0);
  EXPECT_EQ(options.cuboids[0].getH(), 1.1225);
  EXPECT_EQ(options.cuboids[0].getMass(), 1.0);
  EXPECT_EQ(options.cuboids[0].getX(), 40);
  EXPECT_EQ(options.cuboids[0].getY(), 8);
  EXPECT_EQ(options.cuboids[0].getZ(), 1);
  EXPECT_EQ(options.cuboids[0].getMeanBrownianMotion(), 0.1);

  EXPECT_EQ(options.cuboids[1].getInitialVelocity()[0], 0.0);
  EXPECT_EQ(options.cuboids[1].getInitialVelocity()[1], -10.0);
  EXPECT_EQ(options.cuboids[1].getInitialVelocity()[2], 0.0);
  EXPECT_EQ(options.cuboids[1].getLLFC()[0], 15.0);
  EXPECT_EQ(options.cuboids[1].getLLFC()[1], 15.0);
  EXPECT_EQ(options.cuboids[1].getLLFC()[2], 0.0);
  EXPECT_EQ(options.cuboids[1].getH(), 1.1225);
  EXPECT_EQ(options.cuboids[1].getMass(), 1.0);
  EXPECT_EQ(options.cuboids[1].getX(), 8);
  EXPECT_EQ(options.cuboids[1].getY(), 8);
  EXPECT_EQ(options.cuboids[1].getZ(), 1);
  EXPECT_EQ(options.cuboids[1].getMeanBrownianMotion(), 0.1);

  // epsilon
  // sigma
  EXPECT_EQ(options.delta_t, 0.0002);
  EXPECT_EQ(options.writeoutFrequency, 10);
  EXPECT_EQ(options.start, 0);
  EXPECT_EQ(options.end, 5);
  EXPECT_EQ(options.outfile, "simulation");
}