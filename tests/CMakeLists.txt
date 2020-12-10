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
