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

#ifndef INT_CELL_HPP
#define INT_CELL_HPP

#include "new_handler_support.hpp"

/**
 * @brief This is simple class that wraps dynamically allocated int. It is used for practicing memory management in C++.
 * 
 */
class int_cell
{
  union // Union to support custom pool allocation
  {
    int* m_value;
    int_cell* m_next;
  };
  static int_cell* m_headOfList;// Head of free list
  static constexpr std::size_t BLOCK_SIZE = 256U; // Constant number of elements allocated per block
public:
    /// Constructors section
    explicit int_cell();
    int_cell(int initializeValue);
    
    /// Fundamental five functions for memory management
    int_cell(const int_cell& other);
    int_cell& operator=(const int_cell& other);
    int_cell(int_cell&& moved);
    int_cell& operator=(int_cell&& moved);
    ~int_cell();
    
    /// Public API
    inline int value() const { return *m_value; }
    inline int value(int newValue) { *m_value = newValue; }
    
    /// Memory management
    static void* operator new(std::size_t n); // Note: This 2 functions provide memory pool allocation. For memory pool it is guaranteed that max
    static void operator delete(void* deleteObject, std::size_t n); // allocated amount of memory will be equal to maximum number of allocated elements.
};

/// Equality operators
bool operator == (const int_cell& lhs, const int_cell& rhs);
bool operator != (const int_cell& lhs, const int_cell& rhs);

/// Order operators
bool operator < (const int_cell& lhs, const int_cell& rhs);
bool operator > (const int_cell& lhs, const int_cell& rhs);
bool operator <= (const int_cell& lhs, const int_cell& rhs);
bool operator >= (const int_cell& lhs, const int_cell& rhs);

#endif // INT_CELL_HPP
