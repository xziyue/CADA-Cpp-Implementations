project(BinarySearch)
set(CMAKE_CXX_STANDARD 11)

#set(BUILD_BINARY_SEARCH_TEST TRUE)

set(SOURCE_FILES Test.cpp)

if(BUILD_ALL_TESTS OR BUILD_BINARY_SEARCH_TEST)
add_executable(BinarySearch ${SOURCE_FILES})
endif()