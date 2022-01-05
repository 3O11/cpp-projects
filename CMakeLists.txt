cmake_minimum_required (VERSION 3.16)

project ("cpp-projects")

set (CMAKE_CXX_STANDARD 17)

set (CMAKE_CXX_FLAGS "-fsanitize=undefined -fsanitize=leak -fsanitize=address -Wall -Wextra")
set (CMAKE_VERBOSE_MAKEFILE ON)

# Include sub-projects.
add_subdirectory ("01-Counting")
add_subdirectory ("02-MacroProcessor")
add_subdirectory ("03-PolymorphicSort")
