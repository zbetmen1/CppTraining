#ifndef NEW_HANDLER_SUPPORT_HPP
#define NEW_HANDLER_SUPPORT_HPP

#include <new>
#include <cstddef>
#include <stdexcept>

/**
 * @brief Provides support for memory allocation error handling using simple callback functions.
 * 
 */
template <typename T>
class new_handler_support
{
  static std::new_handler m_handle;
public:
  static std::new_handler set_new_handler(std::new_handler handle);
  static void* operator new(std::size_t size);
};

template <typename T>
std::new_handler new_handler_support<T>::set_new_handler(std::new_handler handle)
{
  auto old = m_handle;
  m_handle = handle;
  return old;
}

template <typename T>
void* new_handler_support<T>::operator new(std::size_t size)
{
  auto globalHandle = std::set_new_handler(m_handle);
  void* memory = nullptr;
  try
  {
    memory = ::operator new(size);
  }
  catch (const std::bad_alloc& e)
  {
    std::set_new_handler(globalHandle);
    throw;
  }
  std::set_new_handler(globalHandle);
  return memory;
}


/**
 * @brief Initialize static member of template class to nullptr.
 * 
 */
template<typename T>
std::new_handler new_handler_support<T>::m_handle;

#endif // NEW_HANDLER_SUPPORT_HPP