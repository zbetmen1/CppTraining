#ifndef POOL_ALLOCATOR_HPP
#define POOL_ALLOCATOR_HPP

#include <cstddef>
#include <limits>
#include <iostream>
#include "memory_pool.hpp"

/// Statistics for pool memory allocator

class pool_allocator_statistics 
{
protected:
  static std::size_t m_singleElementAllocations;
  static std::size_t m_arrayAllocations;
  static std::size_t m_singleElementDeallocations;
  static std::size_t m_arrayDeallocations;
  
  inline static void reset() 
  { 
    m_singleElementAllocations = m_arrayAllocations = 
    m_singleElementDeallocations = m_arrayDeallocations = 0U;
  }
  
public:
  inline static void print(std::ostream& out = std::cout)
  {
    std::cout << "Number of single element allocations performed: " << m_singleElementAllocations << '\n'
              << "Number of array block allocations performed: " << m_arrayAllocations << '\n'
              << "Number of single element deallocations performed: " << m_singleElementDeallocations << '\n'
              << "Number of array block deallocations performed: " << m_arrayDeallocations << std::endl;
  }
};

template <typename T>
class pool_allocator: public pool_allocator_statistics
{
  static memory_pool m_allocator;
public:
  /// Standard library allocator type interface
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  
  using pointer = T*;
  using const_pointer = const T*;
  
  using reference = T&;
  using const_reference = const T&;
  
  template <typename U> struct rebind { using other = pool_allocator<U>; };
  
  inline pointer address(reference r) { return &r; }
  inline const_pointer address(const_reference r) { return &r; }
  
  pool_allocator() {}
  ~pool_allocator() {}
  
  pointer allocate(size_type n, pool_allocator<T>::const_pointer hint = nullptr);
  void deallocate(pointer p, size_type n);
  
  inline void construct(pointer p, const T& val) { new(p) T(val); }
  inline void destroy(pointer p) { p->~T(); }
  
  size_type max_size() const { return std::numeric_limits<size_type>::max(); };
};

template <typename T>
memory_pool pool_allocator<T>::m_allocator{sizeof(T)};

template <typename T>
typename pool_allocator<T>::pointer pool_allocator<T>::allocate(size_type n, pool_allocator< T >::const_pointer hint)
{
  if (1U == n) 
  {
    ++pool_allocator_statistics::m_singleElementAllocations;
    return static_cast<typename pool_allocator<T>::pointer>(pool_allocator<T>::m_allocator.allocate());
  }
  else
  {
    ++pool_allocator_statistics::m_arrayAllocations;
    return new T[n];
  }
}

template <typename T>
void pool_allocator<T>::deallocate(pool_allocator<T>::pointer p, size_type n)
{
  if (1U == n) 
  {
    ++pool_allocator_statistics::m_singleElementDeallocations;
    pool_allocator<T>::m_allocator.deallocate(p);
  }
  else
  {
    ++pool_allocator_statistics::m_arrayDeallocations;
    delete[] p;
  }
}

#endif // POOL_ALLOCATOR_HPP