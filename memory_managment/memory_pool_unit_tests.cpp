#include <gtest/gtest.h>
#include <cstddef>
#include <list>
#include "memory_pool.hpp"
#include "pool_allocator.hpp"

using namespace std;

static constexpr size_t SIZE = 1 << 22;

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

TEST(PoolAllocator, ListDouble)
{
  list<double, pool_allocator<double>> l;
  for (size_t i = 0U; i < SIZE; ++i)
  {
    l.push_back(i);
  }
}

TEST(PoolAllocator, ListDoubleDefault)
{
  list<double> l;
  for (size_t i = 0U; i < SIZE; ++i)
  {
    l.push_back(i);
  }
}

TEST(PoolAllocator, ListInt)
{
  list<int, pool_allocator<int>> l;
  for (size_t i = 0U; i < SIZE; ++i)
  {
    l.push_back(i);
  }
}