#include <gtest/gtest.h>
#include "int_cell.hpp"

TEST(IntCellTest, ExplicitConstruction)
{
  int_cell cell{};
  ASSERT_EQ(cell.value(), 0);
}

TEST(IntCellTest, ConstructionFromValue)
{
  int_cell cell(13);
  ASSERT_EQ(cell.value(), 13);
}

TEST(IntCellTest, Equality)
{
  int_cell cell1{};
  int_cell cell2{};
  ASSERT_EQ(cell1, cell2);
}

TEST(IntCellTest, Inequality)
{
  int_cell cell1{13};
  int_cell cell2{};
  ASSERT_NE(cell1, cell2);
}

TEST(IntCellTest, NewTest)
{
  for (int i = 0; i < 256; ++i)
    int_cell* p = new int_cell;
  int_cell* pover = new int_cell;
}

TEST(IntCellTest, AllocateArray)
{
  int_cell* array = new int_cell[256];
  delete[] array;
}