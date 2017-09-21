cmake_minimum_required(VERSION 3.8)
project(CADA-Cpp-Implementations)

set(CMAKE_CXX_STANDARD 11)

include_directories(Misc)

add_subdirectory("Chapter 2/2-3 Binary Search/")
add_subdirectory("Chapter 2/2-6 Chessboard/")

#set(SOURCE_FILES main.cpp)
#add_executable(CADA-Cpp-Implementations ${SOURCE_FILES})