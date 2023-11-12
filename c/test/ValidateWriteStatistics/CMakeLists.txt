set(SOURCE_PATH ../../src)

set(TEST_CASES
    test_StatsFileNotExists
    test_StatsFileNoReadPermission
    test_StatsGoodBmp
    test_StatsGoodRegularFile
    test_StatsGoodDirectory
    test_StatsGoodSymlink
)

foreach(TEST_CASE ${TEST_CASES})
    add_executable(${TEST_CASE} ${TEST_CASE}.c ${SOURCE_PATH}/statistics.c ${SOURCE_PATH}/generals.c)
    target_include_directories(${TEST_CASE} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
    target_link_libraries(${TEST_CASE} PRIVATE unity)
    add_test(NAME ${TEST_CASE} COMMAND ${TEST_CASE})
endforeach()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/bmp-no-read.bmp 
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
    FILE_PERMISSIONS OWNER_WRITE)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources-test DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
