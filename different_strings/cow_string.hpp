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

#ifndef COW_STRING_H
#define COW_STRING_H

#include <cstddef>
#include <iostream>

class cow_string
{
  struct cow_string_buff 
  {
    // Declare members 
    std::size_t m_len;
    std::size_t m_used;
    std::size_t m_refs;
    char* m_buff;
    
    // Default construction, construction from C string and destruction
    cow_string_buff();
    cow_string_buff(const char* cstr);
    ~cow_string_buff();
    
    // This type shouldn't be copyable so forbid copying
    cow_string_buff(const cow_string_buff& oth);
    cow_string_buff& operator=(const cow_string_buff& oth);
    
    // This type must be movable so implement move semantics
    cow_string_buff(cow_string_buff&& oth);
    cow_string_buff& operator=(cow_string_buff&& oth);
    
    // Utility function used to reserve memory for n characters. Does nothing if there's enough memory already.
    void reserve(std::size_t n);
  };
  
  cow_string_buff* m_strrep;
  static constexpr std::size_t UNSHAREABLE = 0U;
public:
  // Default construction, construction and assignment from C string
  cow_string();
  cow_string(const char* cstr);
  cow_string& operator=(const char* cstr);
  
  // Copying, moving semantics and destruction
  cow_string(const cow_string& other);
  cow_string& operator=(const cow_string& other);
  cow_string(cow_string&& other);
  cow_string& operator=(cow_string&& other);
  ~cow_string();
  
  // Access operators
  const char& operator[](std::size_t i) const;
  char& operator[](std::size_t i);
  
  // String operations
  void append(char c);
  inline void push_back(char c) { append(c); }
  inline bool share_buffer(const cow_string& oth) const { return m_strrep->m_buff == oth.m_strrep->m_buff; }
  inline void clear() { ensure_unique_buffer(m_strrep->m_used); m_strrep->m_used = 0U; }
  
  // I/O operators
  friend std::ostream& operator<<(std::ostream& out, const cow_string& str);
  friend std::istream& operator>>(std::istream& in, cow_string& str);
private:
  void decrement_and_release();
  void ensure_unique_buffer(std::size_t n, bool markUnshareable = false);
};


#endif // COW_STRING_H
