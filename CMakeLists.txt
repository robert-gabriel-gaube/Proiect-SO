cmake_minimum_required(VERSION 3.16)

project(OS_Project)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

set(SOURCES src/main.c
            src/generals.c
            src/process_directory.c
            src/process_file.c
            src/file_type.c
            src/statistics.c
            src/grayscale_filter.c)

add_executable(program ${SOURCES})
target_include_directories(program PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
            
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/sentence-counter.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})