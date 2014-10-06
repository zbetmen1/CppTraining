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

#ifndef SSO_STRING_H
#define SSO_STRING_H

#include <cstddef>
#include <gtest/gtest.h>

class sso_string
{
  static constexpr std::size_t STACK_SIZE = 16U; // This is size of stack used to store small string (note that this is 15 characters)
  static constexpr std::size_t DATA_ON_STACK_LEN = 0U; // If data is stored on stack "m_len" field has this value.
  
  std::size_t m_len;
  std::size_t m_used;
  union
  {
    char* m_buffer;
    char m_stack[STACK_SIZE];
  };

  class riter
  {
    char* m_current;
  public:
    riter(char* current)
    : m_current{current}
    {}
    
    char& operator*() { return *m_current; }
    const char& operator*() const { return *m_current; }
    
    char* operator->() { return m_current; }
    const char* operator->() const { return m_current; }
    
    riter operator++() { return riter{--m_current}; }
    riter operator++(int) { return riter{--m_current}; }
    
    riter operator--() { return riter{++m_current}; }
    riter operator--(int) { return riter{++m_current}; }
  };
public:
  using iterator = char*;
  using const_iterator = const char*;
  using reverse_iterator = riter;
  using const_reverse_iterator = const riter;
  
  sso_string();
  sso_string(const char* cstr);
  
  sso_string(const sso_string& oth);
  sso_string(sso_string&& oth);
  sso_string& operator=(const sso_string& oth);
  sso_string& operator=(sso_string&& oth);
  ~sso_string();
  
  inline bool empty() const { return 0U == m_used; }
  inline const char* c_str() const { return data_on_stack() ? &m_stack[0] : m_buffer; }
  inline size_t capacity() const { return data_on_stack() ? STACK_SIZE : m_len; }
  inline size_t size() const { return m_used; }
  
  char& operator[](std::size_t i) { return data_on_stack() ? m_stack[i] : m_buffer[i]; }
  const char& operator[](std::size_t i) const 
  { 
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-local-addr"
    return data_on_stack() ? m_stack[i] : m_buffer[i];
#pragma GCC diagnostic pop
  }
  
  void reserve(std::size_t n);
  void push_back(char c);
  void pop_back() { if (0U == m_used) return; else (data_on_stack() ? m_stack[--m_used] : m_buffer[--m_used]) = '\0'; }
  
  char& front() { return operator[](0U); }                   // When called on an
  const char& front() const { return operator[](0U); }       // empty sso_string
  char& back() { return operator[](m_used - 1); }            // behavior is undefined
  const char& back() const { return operator[](m_used - 1); }// by C++ standard so it is here.
  
  iterator begin() { return data_on_stack() ? &m_stack[0] : m_buffer; }
  iterator end() { return data_on_stack() ? &m_stack[m_used] : m_buffer + m_used; }
  const_iterator cbegin() const { return data_on_stack() ? &m_stack[0] : m_buffer; }
  const_iterator cend() const { return data_on_stack() ? &m_stack[m_used] : m_buffer + m_used; }
  reverse_iterator rbegin() { return 0U == m_used ? rend() : riter{data_on_stack() ? &m_stack[m_used - 1] : &m_buffer[m_used - 1]}; }
  reverse_iterator rend() { return riter{data_on_stack() ? &m_stack[-1] : &m_buffer[-1]}; }
  const_reverse_iterator crbegin() const { return 0U == m_used ? crend() : riter{data_on_stack() ? const_cast<char*>(&m_stack[m_used - 1]) : const_cast<char*>(&m_buffer[m_used - 1])}; }
  const_reverse_iterator crend() const { return riter{data_on_stack() ? const_cast<char*>(&m_stack[-1]) : const_cast<char*>(&m_buffer[-1])}; }
  
  friend bool operator==(const sso_string& l, const sso_string& r);
  friend bool operator!=(const sso_string& l, const sso_string& r);
  friend bool operator==(const sso_string& l, const char* r);
  friend bool operator!=(const sso_string& l, const char* r);
  friend bool operator==(const char* l, const sso_string& r);
  friend bool operator!=(const char* l, const sso_string& r);
  friend bool compare_normal(const sso_string& str, const char* cstr, std::size_t n);
  friend bool compare_normal(const sso_string& str, const sso_string& oth, std::size_t n);
  
  // TODO: Remove friend test when done testing
  FRIEND_TEST(SsoString, DataOnStack);
  FRIEND_TEST(SsoString, Reserve);
private:
  void init(const char* cstr, std::size_t cstrLen, char* buffer, std::size_t bufferLen) noexcept;
  inline bool data_on_stack() const { return m_len <= m_used; }
};

#endif // SSO_STRING_H
