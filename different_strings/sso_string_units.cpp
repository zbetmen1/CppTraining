#include <gtest/gtest.h>
#include <cstring>
#include <string>
#include <vector>

#include "sso_string.hpp"

using namespace std;

TEST(SsoString, DataOnStack)
{
  // Test on creation short
  sso_string shortStr{"Short string!"};
  ASSERT_EQ(shortStr.data_on_stack(), true);
  
  // Test on creation long
  sso_string longStr{"Very, very long string!"};
  ASSERT_EQ(longStr.data_on_stack(), false);
  
  // Copy long string to short
  shortStr = longStr;
  ASSERT_EQ(shortStr.data_on_stack(), false);
  ASSERT_EQ(shortStr == longStr, true);
  
  // Copy short string to long string
  longStr = sso_string{"Shortest!"};
  ASSERT_EQ(longStr.data_on_stack(), false);
  ASSERT_EQ(longStr == "Shortest!", true);
  
  // Create two short strings and copy one to other
  sso_string firstShortStr{"Shorty one!"};
  sso_string secondShortStr{"Shorty two!"};
  ASSERT_EQ(firstShortStr.data_on_stack(), true);
  ASSERT_EQ(secondShortStr.data_on_stack(), true);
  firstShortStr = secondShortStr;
  ASSERT_EQ(firstShortStr.data_on_stack(), true);
  ASSERT_EQ(firstShortStr == secondShortStr, true);
}

TEST(SsoString, Reserve)
{
  // Reserve string on heap, check that it is empty and that data is on heap (also check capacity)
  sso_string heapStr;
  ASSERT_EQ(heapStr.data_on_stack(), true);
  size_t capacity = sso_string::STACK_SIZE + 1;
  heapStr.reserve(capacity);
  ASSERT_EQ(heapStr.capacity() >= capacity, true); // reserve will reserve at least capacity spaces for chars
  ASSERT_EQ(heapStr.data_on_stack(), false);
  ASSERT_EQ(heapStr.empty(), true);
}

TEST(SsoString, PushBack)
{
  sso_string str;
  string stdstr{"some nice text!"};
  for (char c : stdstr)
  {
    str.push_back(c);
  }
  ASSERT_EQ(str == stdstr.c_str(), true);
  ASSERT_EQ(str.size(), stdstr.size());
}

TEST(SsoString, PopBack)
{
  sso_string str{"A lot of chars to pop!"};
  vector<sso_string> poped = { 
                                sso_string{"A lot of chars to pop"}, 
                                sso_string{"A lot of chars to po"},
                                sso_string{"A lot of chars to p"},
                                sso_string{"A lot of chars to "},
                                sso_string{"A lot of chars to"},
                                sso_string{"A lot of chars t"},
                                sso_string{"A lot of chars "},
                                sso_string{"A lot of chars"},
                                sso_string{"A lot of char"},
                                sso_string{"A lot of cha"},
                                sso_string{"A lot of ch"},
                                sso_string{"A lot of c"},
                                sso_string{"A lot of "},
                                sso_string{"A lot of"},
                                sso_string{"A lot o"},
                                sso_string{"A lot "},
                                sso_string{"A lot"},
                                sso_string{"A lo"},
                                sso_string{"A l"},
                                sso_string{"A "},
                                sso_string{"A"},
                                sso_string{""}
                             };
  str.pop_back();
  for (const sso_string& elem : poped)
  {
    ASSERT_EQ(elem == str, true);
    str.pop_back();
  }
  ASSERT_EQ(str.empty(), true);
}