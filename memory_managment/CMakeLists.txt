cmake_minimum_required(VERSION 2.8)

project(memory_managment)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(memory_managment int_cell_unit_tests.cpp int_cell.cpp main.cpp)

# Link with gtest
target_link_libraries(memory_managment gtest pthread)
install(TARGETS memory_managment RUNTIME DESTINATION bin)
