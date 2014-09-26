#include <iostream>
#include <gtest/gtest.h>
#include "int_cell.hpp"

using namespace std;

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  auto testResults = RUN_ALL_TESTS();
  return 0;
}