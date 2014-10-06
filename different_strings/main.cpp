#include <iostream>
#include <gtest/gtest.h>

#include "cow_string.hpp"
#include "sso_string.hpp"

using namespace std;

int main(int argc, char **argv) 
{
  testing::InitGoogleTest(&argc, argv);
  auto testResults = RUN_ALL_TESTS();
  return 0;
}
