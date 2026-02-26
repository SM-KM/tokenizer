cmake_minimum_required(VERSION 4.2)
project(token VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_WARN_DEPRECATED TRUE)

option(DEBUG "Debug generator and contructor" OFF)
if(DEBUG)
    message(STATUS "Setting sources")
endif()

set(SOURCES
    ./src/token.h
)

set(ENTRY_POINT
    ./src/main.cpp
)

if(DEBUG)
    message(STATUS "entry_point set to: ${ENTRY_POINT}")
endif()

add_executable(tkn ${SOURCES} ${ENTRY_POINT})

# test suite using google test
option(BUILD_TESTS "Build unit test" OFF)
if(BUILD_TESTS) 
    include(FetchContent)
    FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
    FetchContent_MakeAvailable(googletest)
    enable_testing()
    add_executable(tests ${APP_SOURCES} ./tests/util.cc)
    target_link_libraries(
	tests PRIVATE
	gtest_main
	${LIBS}	
    )
    include(GoogleTest)
    gtest_discover_tests(tests)
endif()
