project(Chessboard)
set(CMAKE_CXX_STANDARD 11)

set(BUILD_CHESSBOARD_TEST TRUE)

set(SOURCE_FILES Test.cpp)

if(BUILD_ALL_TESTS OR BUILD_CHESSBOARD_TEST)
add_executable(Chessboard ${SOURCE_FILES})
endif()