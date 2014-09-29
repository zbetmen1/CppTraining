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

#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <cstddef>

/**
 * @brief This memory pool class. It is used to allocate and deallocate memory for small objects
 * efficiently. This type is not copyable and it's not copy-assignable. Memory pool can be moved.
 * 
 */
class memory_pool
{
  /// Types and constants for memory pool
  using byte = unsigned char;
  static constexpr std::size_t BLOCK_BYTES = 4096U;
  
  /**
   * @brief This struct represents one memory block that will be allocated and that will know about next block in line.
   * 
   */
  struct memory_block 
  {
    byte m_block[BLOCK_BYTES];
    memory_block* m_nextBlock;
  };
  
  /**
   * @brief This struct worries about free memory list from which memory is assigned to new objects.
   * 
   */
  struct link 
  {
    link* m_nextLink;
  };
  
  std::size_t m_n; // Size of elements that will be allocated. Note that it is not read only field cause of move-assignment.
  memory_block* m_blocks; // We need to maintain list of allocated blocks so we can release allocated memory
  link* m_headOfFreeList; // We need to partition the allocated memory to linked list. This is head of free memory list. 
public:
  /// Complete public interface for memory pool class
  memory_pool(std::size_t n);
  ~memory_pool();
  void* allocate();
  void deallocate(void* memory);
  
  /// Implement move semantics
  memory_pool(memory_pool&& moved);
  memory_pool& operator = (memory_pool&& moved);
  
  /// Forbid copying and copy-assigning
  memory_pool(const memory_pool&) = delete;
  memory_pool& operator = (const memory_pool&) = delete;
private:
  void free_allocated_memory();
};

#endif // MEMORY_POOL_H
