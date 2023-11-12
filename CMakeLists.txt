cmake_minimum_required(VERSION 3.16)

project(OS_Project)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

option(BUILD_TESTS "Build tests" OFF)
option(DEBUG "Debug mode" OFF)

if(DEBUG)
    add_definitions(-DDEBUG=1)
endif()

set(SOURCES c/src/main.c
            c/src/generals.c
            c/src/statistics.c
            c/src/process_directory.c)

add_executable(program ${SOURCES})
target_include_directories(program PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/c/include)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(c/test)
endif()

