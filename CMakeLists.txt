cmake_minimum_required(VERSION 3.18)

project(OS_Project)
set(SOURCES c/src/main.c
            c/src/generals.c
            c/src/statistics.c)

add_executable(program ${SOURCES})
target_include_directories(program PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/c/include)
target_sources(program PRIVATE )

