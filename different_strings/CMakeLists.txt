cmake_minimum_required(VERSION 2.8)

project(different_strings)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(different_strings sso_string_units.cpp sso_string.cpp cow_string.cpp main.cpp)

# Link with gtest
target_link_libraries(different_strings gtest pthread)

install(TARGETS different_strings RUNTIME DESTINATION bin)
