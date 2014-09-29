#include <gtest/gtest.h>
#include <cstddef>
#include "memory_pool.hpp"

using namespace std;

static constexpr size_t SIZE = 1 << 24;

TEST(MemoryPool, AllocatingDoubles)
{
  memory_pool pool(sizeof(double));
  for (size_t i = 0U; i < SIZE; ++i)
  {
    double* p = static_cast<double*>(pool.allocate());
  }
}

TEST(MemoryPool, ElementByElementAllocationDouble)
{
  for (size_t i = 0U; i < SIZE; ++i)
  {
    double* p = new double;
    delete p;
  }
}