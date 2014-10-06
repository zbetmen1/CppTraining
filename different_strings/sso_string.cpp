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

#include "sso_string.hpp"

#include <cstring>
#include <algorithm>
#include <utility>

using namespace std;

sso_string::sso_string()
: m_len{0U}, m_used{0U}, m_buffer{nullptr}
{
}

sso_string::sso_string(const char* cstr)
{
  auto cstrLen = strlen(cstr);
  std::size_t bufferLen = 0U;
  char* buffer = nullptr;
  if (cstrLen < STACK_SIZE) 
  {
    bufferLen = DATA_ON_STACK_LEN;
    buffer = m_stack;
  }
  else
  {
    bufferLen = cstrLen + 1;
    buffer = m_buffer = new char[bufferLen];
  }
  init(cstr, cstrLen, buffer, bufferLen);
}

sso_string::sso_string(const sso_string& oth)
: m_len{oth.m_len}, m_used{oth.m_used}
{
  // Check if data can fit onto stack and copy data to stack if it can
  if (m_used < STACK_SIZE) 
  {
    // Find out from where to copy data (stack or heap of oth)
    if (oth.data_on_stack()) 
      copy_n(oth.m_stack, oth.m_used, m_stack);
    else
      copy_n(oth.m_buffer, oth.m_used, m_stack);
    m_stack[m_used] = '\0';
    m_len = DATA_ON_STACK_LEN; // Adjust m_len for this cause memory is on stack
  }
  else
  {
    m_buffer = new char[m_len]; // m_len is set correctly in initialization list
    copy_n(oth.m_buffer, oth.m_used, m_buffer); 
    m_buffer[m_used] = '\0';
  }
}

sso_string::sso_string(sso_string&& oth)
: m_len{oth.m_len}, m_used{oth.m_used}
{
  // If data is on stack we can just copy it. Otherwise we can take ownership of oth heap memory.
  if (oth.data_on_stack())
  {
    copy_n(oth.m_stack, oth.m_used, m_stack);
    m_stack[m_used] = '\0';
  }
  else
  {
    m_buffer = oth.m_buffer;
    oth.m_buffer = nullptr;
  }
}

sso_string& sso_string::operator=(const sso_string& oth)
{
  // Self test
  if (&oth == this) return *this;
  
  char* buffer = nullptr;
  size_t bufferLen = 0U;
  
  if (data_on_stack())
  {
    // When data from oth can fit stack use it! Otherwise switch this storage to heap storage.
    if (oth.m_used < STACK_SIZE)
    {
      bufferLen = DATA_ON_STACK_LEN;
      buffer = m_stack;
    }
    else
    {
      bufferLen = oth.m_used + 1;
      buffer = new char[bufferLen]; // This is the only place where exception can arise, and this isn't modified until next line,
      m_buffer = buffer;            // so this should be fine when in matters of exception safety.
    }
  }
  else
  {
    // Data is on heap so we wanna reserve at least enough memory to perform data copy and add terminal zero at the end of buffer
    reserve(oth.m_used + 1);
    bufferLen = max(oth.m_used + 1, m_len); // If current m_len is greater than potential new m_len use old m_len
    buffer = m_buffer;
  }
  
  // Find out which storage oth uses and copy data
  init(oth.data_on_stack() ? oth.m_stack : oth.m_buffer, oth.m_used, buffer, bufferLen);
  return *this;
}

sso_string& sso_string::operator=(sso_string&& oth)
{
  // Self test
  if (&oth == this) return *this;
  
  // We can't move data since it's on the stack - we must copy it
  if (oth.data_on_stack())
  {
    char* buffer = nullptr;
    size_t bufferLen = 0U;
    
    // Find out where to copy data
    if (data_on_stack())
    {
      buffer = m_stack;
      bufferLen = DATA_ON_STACK_LEN;
    }
    else
    {
      buffer = m_buffer;
      bufferLen = max(oth.m_used + 1, m_len);
    }
    init(oth.m_stack, oth.m_used, buffer, bufferLen);
  }
  else // We can move data!
  {
    // Check if we are already using heap we must cleanup resources
    if (!data_on_stack())
      delete[] m_buffer;
    
    // Assign oth data to this
    m_len = oth.m_len;
    m_used = oth.m_used;
    m_buffer = oth.m_buffer;
    
    // Set oth pointer to avoid double delete on m_buffer
    oth.m_buffer = nullptr;
  }
  return *this;
}

sso_string::~sso_string()
{
  if (!data_on_stack())
    delete[] m_buffer;
}

void sso_string::reserve(std::size_t n)
{
  // If we need to allocate space less characters than fit onto stack just return
  if (n < STACK_SIZE) return;
  
  // We'll reserve more space only if we already don't have enough
  if (m_len < n + 1)
  {
    size_t reserveSpace = max<size_t>(1.5 * m_len, n + 1);
    char* buffer = new char[reserveSpace]; // also reserving space for terminal zero
    
    // Handle differences when current data is on stack and when current data is on heap
    if (data_on_stack()) 
    {
      copy_n(m_stack, m_used, buffer);
      m_buffer = buffer;
    }
    else
    {
      copy_n(m_buffer, m_used, buffer);
      swap(buffer, m_buffer);
      delete[] buffer;
    }
    m_len = reserveSpace;
  }
}

void sso_string::push_back(char c)
{
  reserve(m_used + 1);
  if (data_on_stack())
  {
    m_stack[m_used++] = c;
    m_stack[m_used] = '\0';
  }
  else 
  {
    m_buffer[m_used++] = c;
    m_buffer[m_used] = '\0';
  }
}

void sso_string::init(const char* cstr, std::size_t cstrLen, char* buffer, std::size_t bufferLen) noexcept
{
  copy_n(cstr, cstrLen, buffer);
  buffer[cstrLen] = '\0';
  m_len = bufferLen;
  m_used = cstrLen;
}

bool operator==(const sso_string& l, const sso_string& r)
{
  // Check that strings are of same size
  auto sizeL = l.size();
  auto sizeR = r.size();
  if (sizeL != sizeR) return false;
  
  // Compare 
  return compare_normal(l, r, sizeL);
}

bool operator!=(const sso_string& l, const sso_string& r)
{
  return !(l == r);
}

bool operator==(const sso_string& l, const char* r)
{
  auto sizeL = l.size();
  auto sizeR = strlen(r);
  if (sizeL != sizeR) return false;
  
  return compare_normal(l, r, sizeL);
}

bool operator!=(const sso_string& l, const char* r)
{
  return !(l == r);
}

bool operator==(const char* l, const sso_string& r)
{
  auto sizeL = strlen(l);
  auto sizeR = r.size();
  if (sizeL != sizeR) return false;
  
  return compare_normal(r, l, sizeL);
}

bool operator!=(const char* l, const sso_string& r)
{
  !(l == r);
}

bool compare_normal(const sso_string& str, const char* cstr, std::size_t n)
{
  // Check strings char by char
  for (size_t i = 0U; i < n; ++i)
  {
    if (str[i] != cstr[i])
      return false;
  }
  return true;
}

bool compare_normal(const sso_string& str, const sso_string& oth, std::size_t n)
{
  // Check strings char by char
  for (size_t i = 0U; i < n; ++i)
  {
    if (str[i] != oth[i])
      return false;
  }
  return true;
}


