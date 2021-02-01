cmake_minimum_required(VERSION 3.13)

###########################################
################# targets #################
###########################################

add_executable(XIAengine_test XIAengine.cpp ${CMAKE_SOURCE_DIR}/src/XIAengine/functions.cpp)
target_link_libraries(XIAengine_test doctest::doctest ${PROJECT_NAME}::xia inih::inih)
target_compile_definitions(XIAengine_test PRIVATE INI_PATH="${CMAKE_SOURCE_DIR}/template/pxisys.ini" CMAKE_TEST)
add_test(
        NAME XIAengine.test
        COMMAND XIAengine_test ${TEST_RUNNER_PARAMS}
)

add_executable(mergesorter_test mergesorter.cpp)
set_target_properties(mergesorter_test PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)
target_link_libraries(mergesorter_test ${PROJECT_NAME}::mergesorter doctest::doctest)
add_test(
        NAME mergesorter.test
        COMMAND mergesorter_test ${TEST_RUNNER_PARAMS}
)