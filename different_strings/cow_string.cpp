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

#include "cow_string.hpp"

#include <cstring>
#include <algorithm>
#include <utility>
#include <memory>
#include <iterator>
#include <string>

using namespace std;

/// Implement Copy-On-Write string buffer

cow_string::cow_string_buff::cow_string_buff()
: m_len{0U}, m_used{0U}, m_refs{1U}, m_buff{nullptr}
{
}

cow_string::cow_string_buff::cow_string_buff(const char* cstr)
: m_len{::strlen(cstr)}, m_used{m_len}, m_refs{1U}, m_buff{new char[m_len]}
{
  copy_n(cstr, m_len, m_buff);
}

cow_string::cow_string_buff::~cow_string_buff()
{
  delete[] m_buff;
}

cow_string::cow_string_buff::cow_string_buff(const cow_string::cow_string_buff& oth)
: m_len{oth.m_len}, m_used{oth.m_used}, m_refs{1U}, m_buff{new char[m_len]}
{
  copy_n(oth.m_buff, oth.m_used, m_buff);
}

cow_string::cow_string_buff& cow_string::cow_string_buff::operator=(const cow_string::cow_string_buff& oth)
{
  if (this == &oth) return *this;
  
  char* newBuff = new char[oth.m_len];
  copy_n(oth.m_buff, oth.m_used, newBuff);
  swap(newBuff, m_buff);
  delete[] newBuff;
  
  m_len = oth.m_len;
  m_used = oth.m_used;
  m_refs = 1U;
}

cow_string::cow_string_buff::cow_string_buff(cow_string::cow_string_buff&& oth)
: m_len{oth.m_len}, m_used{oth.m_used}, m_refs{oth.m_refs}, m_buff{oth.m_buff}
{ 
  oth.m_buff = nullptr;
  oth.m_len = oth.m_used = 0U;
  oth.m_refs = 1U;
}

cow_string::cow_string_buff& cow_string::cow_string_buff::operator=(cow_string::cow_string_buff&& oth)
{
  // Release resource and get values from oth
  delete[] m_buff;
  m_len = oth.m_len;
  m_used = oth.m_used;
  m_refs = oth.m_refs;
  m_buff = oth.m_buff;
  
  // Modify other to safe state
  oth.m_len = 0U;
  oth.m_used = 0U;
  oth.m_refs = 1U;
  oth.m_buff = nullptr;
}

void cow_string::cow_string_buff::reserve(std::size_t n)
{
  if (m_len < n) 
  {
    size_t newLen = max( static_cast<size_t>(1.5 * m_len), n);
    char* newBuff = new char[newLen];
    copy_n(m_buff, m_used, newBuff);
    swap(newBuff, m_buff);
    m_len = newLen;
    delete[] newBuff;
  }
}

/// Implement Copy-On-Write string

cow_string::cow_string()
: m_strrep{new cow_string_buff}
{
}

cow_string::cow_string(const char* cstr)
: m_strrep{new cow_string_buff{cstr}}
{
}

cow_string& cow_string::operator=(const char* cstr)
{
  decrement_and_release();
  m_strrep = new cow_string_buff{cstr};
  return *this;
}

cow_string::cow_string(const cow_string& other)
{
  if (cow_string::UNSHAREABLE == other.m_strrep->m_refs)
  {
    m_strrep = new cow_string_buff{*other.m_strrep};
  }
  else
  {
    m_strrep = other.m_strrep;
    ++m_strrep->m_refs;
  }
}

cow_string::cow_string(cow_string&& other)
: m_strrep{other.m_strrep}
{
  other.m_strrep = nullptr;
}

cow_string& cow_string::operator=(const cow_string& other)
{
  // Check self assignment
  if (this == &other) return *this;
  
  // Change m_strrep to reference other.m_strrep and increment number of references to it
  decrement_and_release();
  m_strrep = other.m_strrep;
  ++m_strrep->m_refs;
  return *this;
}

cow_string& cow_string::operator=(cow_string&& other)
{
  // Check self assignment
  if (this == &other) return *this;
  
  decrement_and_release();
  m_strrep = other.m_strrep;
  other.m_strrep = nullptr;
  return *this;
}

void cow_string::decrement_and_release()
{
  // If I'm unshareable or the only object referencing current m_strrep delete it
  if (m_strrep->m_refs == cow_string::UNSHAREABLE || --m_strrep->m_refs == 0U)
  {
    delete m_strrep;
  }
}

cow_string::~cow_string()
{
  decrement_and_release();
}

const char& cow_string::operator[](std::size_t i) const
{
  return m_strrep->m_buff[i];
}

char& cow_string::operator[](std::size_t i)
{
  ensure_unique_buffer(m_strrep->m_used, true);
  return m_strrep->m_buff[i];
}

void cow_string::ensure_unique_buffer(std::size_t n, bool markUnshareable)
{
  if (m_strrep->m_refs > 1U) 
  {
    unique_ptr<cow_string_buff> newBuff{new cow_string_buff{}};
    newBuff->reserve(n);
    copy_n(m_strrep->m_buff, m_strrep->m_used, newBuff->m_buff);
    newBuff->m_used = m_strrep->m_used;
    
    --m_strrep->m_refs;
    m_strrep = newBuff.release();
  }
  else
  {
    m_strrep->reserve(n);
  }
  
  m_strrep->m_refs = markUnshareable ? cow_string::UNSHAREABLE : 1U;
}

void cow_string::append(char c)
{
  ensure_unique_buffer(m_strrep->m_used + 1);
  m_strrep->m_buff[m_strrep->m_used++] = c;
}

ostream& operator<<(ostream& out, const cow_string& str)
{
  copy_n(str.m_strrep->m_buff, str.m_strrep->m_used, ostream_iterator<char>{out});
  return out;
}

istream& operator>>(istream& in, cow_string& str)
{
  string helper;
  in >> helper;
  str = helper.c_str();
  return in;
}
