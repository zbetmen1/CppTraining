/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright 2014  <copyright holder> <email>
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

#include "memory_pool.hpp"
#include <utility>

/// Public interface implementation

memory_pool::memory_pool(std::size_t n)
: m_n{n}, m_blocks{nullptr}, m_headOfFreeList{nullptr}
{
}

memory_pool::~memory_pool()
{
  free_allocated_memory();
}

void* memory_pool::allocate()
{
  // If we don't have free memory allocate another memory block
  if (!m_headOfFreeList) 
  {
    // Allocate and add memory block to list of allocated memory blocks
    memory_block* newBlock = new memory_block;
    newBlock->m_nextBlock = m_blocks;
    m_blocks = newBlock;
    
    // Partition memory (note that range is [first, last] which is unusual for STL)
    const auto numOfElements = BLOCK_BYTES / m_n;
    byte* first = newBlock->m_block;
    byte* last = &first[(numOfElements - 1) * m_n];
    while (first != last) 
    {
      reinterpret_cast<link*>(first)->m_nextLink = reinterpret_cast<link*>(first + m_n);
      first += m_n;
    }
    reinterpret_cast<link*>(last)->m_nextLink = nullptr;
    m_headOfFreeList = reinterpret_cast<link*>(newBlock->m_block);
  }
  
  // Relink free list and return pointer to allocated memory
  auto p = m_headOfFreeList;
  m_headOfFreeList = p->m_nextLink;
  return p;
}

void memory_pool::deallocate(void* memory)
{
  auto p = static_cast<link*>(memory);
  p->m_nextLink = m_headOfFreeList;
  m_headOfFreeList = p;
}

/// Move semantics implementation

memory_pool::memory_pool(memory_pool&& moved)
: m_n{moved.m_n}, m_blocks{moved.m_blocks}, m_headOfFreeList{moved.m_headOfFreeList}
{
  moved.m_blocks = nullptr;
  moved.m_headOfFreeList = nullptr;
}

memory_pool& memory_pool::operator=(memory_pool&& moved)
{
  if (this != &moved) 
  {
    // Recreate this using 'moved' values
    free_allocated_memory();
    m_n = moved.m_n;
    m_blocks = moved.m_blocks;
    m_headOfFreeList = moved.m_headOfFreeList;
    
    // Set 'moved' pointers to nullptr
    moved.m_blocks = nullptr;
    moved.m_headOfFreeList = nullptr;
  }
  return *this;
}

void memory_pool::free_allocated_memory()
{
  auto p = m_blocks;
  while (p) 
  {
    auto tmp = p;
    p = p->m_nextBlock;
    delete tmp;
  }
}

