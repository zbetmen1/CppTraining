#include "pool_allocator.hpp"

std::size_t pool_allocator_statistics::m_singleElementAllocations;
std::size_t pool_allocator_statistics::m_arrayAllocations;
std::size_t pool_allocator_statistics::m_singleElementDeallocations;
std::size_t pool_allocator_statistics::m_arrayDeallocations;