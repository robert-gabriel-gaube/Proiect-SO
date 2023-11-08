set(SOURCE_PATH ../../src)

set(TEST_CASES
    test_ArgsWrongNumberOfArgs
    test_ArgsWrongExtension
    test_ArgsNoExtension
    test_ArgsGoodValues
)

foreach(TEST_CASE ${TEST_CASES})
    add_executable(${TEST_CASE} ${TEST_CASE}.c ${SOURCE_PATH}/generals.c)
    target_include_directories(${TEST_CASE} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
    target_link_libraries(${TEST_CASE} PRIVATE unity)
    add_test(NAME ${TEST_CASE} COMMAND ${TEST_CASE})
endforeach()