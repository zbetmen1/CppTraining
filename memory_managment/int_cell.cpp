/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright 2014  Kocić Ognjen zbetmen1@gmail.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "int_cell.hpp"
#include <algorithm>
#include <stdexcept>

using namespace std;

// Initialize head of list
int_cell* int_cell::m_headOfList;

int_cell::int_cell()
{
  m_value = new int[1];
  *m_value = 0;
}

int_cell::int_cell(int initializeValue)
{
  m_value = new int[1];
  *m_value = initializeValue;
}

int_cell::int_cell(const int_cell& other)
{
  m_value = new int[1];
  *m_value = other.value();
}

int_cell& int_cell::operator=(const int_cell& other)
{
  if (this != &other)
  {
    int tmpValue = other.value();
    std::swap(tmpValue, *m_value);
  }
  return *this;
}

int_cell::int_cell(int_cell&& moved)
: m_value(moved.m_value)
{
  moved.m_value = nullptr;
}

int_cell& int_cell::operator=(int_cell&& moved)
{
  if (this != &moved)
  {
    int* tmpCell = moved.m_value;
    moved.m_value = nullptr;
    std::swap(tmpCell, m_value);
    delete tmpCell;
  }
  return *this;
}

int_cell::~int_cell()
{
  delete m_value;
}

void* int_cell::operator new(std::size_t n)
{
  if (sizeof(int_cell) != n) 
  {
    return ::operator new(n);
  }
  
  // When we have enough memory just advance memory list pointer, in other case allocate
  // new block, partition memory, assign return pointer and set new head of free list
  void* returnPtr = static_cast<void*>(m_headOfList);
  if (nullptr == returnPtr) 
  {
    auto newBlock = (int_cell*) ::operator new(BLOCK_SIZE * sizeof(int_cell));
    for (auto i = 1U; i < BLOCK_SIZE - 1; ++i)
    {
      newBlock[i].m_next = &newBlock[i+1];
    }
    newBlock[BLOCK_SIZE-1].m_next = nullptr;
    returnPtr = newBlock;
    m_headOfList = newBlock + 1; 
  }
  else
  {
    m_headOfList = m_headOfList->m_next;
  }
  
  return returnPtr;
}

void int_cell::operator delete(void* deleteObject, std::size_t n)
{
  if (0U == n) 
  {
    return;
  }
  
  if (sizeof(int_cell) == n) 
  {
    auto intCellDeleteObject = (int_cell*) deleteObject;
    intCellDeleteObject->m_next = m_headOfList;
    m_headOfList = intCellDeleteObject;
  }
  else
  {
    ::operator delete(deleteObject);
  }
}

bool operator == (const int_cell& lhs, const int_cell& rhs)
{
  return lhs.value() == rhs.value();
}

bool operator != (const int_cell& lhs, const int_cell& rhs)
{
  return !(lhs == rhs);
}

bool operator < (const int_cell& lhs, const int_cell& rhs)
{
  return lhs.value() < rhs.value();
}

bool operator > (const int_cell& lhs, const int_cell& rhs)
{
  return rhs < lhs;
}

bool operator <= (const int_cell& lhs, const int_cell& rhs)
{
  return !(rhs < lhs);
}

bool operator >= (const int_cell& lhs, const int_cell& rhs)
{
  return !(lhs < rhs);
}