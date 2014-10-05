cmake_minimum_required(VERSION 2.8)

project(different_strings)

add_executable(different_strings cow_string.cpp main.cpp)

install(TARGETS different_strings RUNTIME DESTINATION bin)
