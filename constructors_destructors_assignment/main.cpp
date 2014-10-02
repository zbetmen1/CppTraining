#include <iostream>

/**
 * @brief Demonstrates gcc-4.9 support for new auto return value functions.
 * 
 * @param x Value to be incremented.
 * @return int
 */
auto increment(auto x)
{
  return ++x;
}

/**
 * @brief Demonstration of new VLA feature that is added to C++ gcc implementation. Note that
 * VLA is not part of c++14 standard, but it will be proposed for c++17 very likely.
 * 
 * @param n Size of VLA.
 * @return void
 */
void vla(int n)
{
  int vlaArray[n];
  for (int i = 0; i < n; ++i)
    vlaArray[i] = i;
}

const class /*"my_nullptr" is const variable of anonymous class*/
{
public:
  template<typename T>
  operator T*() const { return 0; } 
  
  template <class C, class T>
  operator T C::*() { return 0; } // Expression T C::* should be read "pointer of type T member of class/struct C".
  
  void operator &() const = delete;
} my_nullptr;

int main(int argc, char **argv) 
{
  std::cout << "Hello, world!" << std::endl;
  return 0;
}
