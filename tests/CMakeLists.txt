# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()


include_directories (
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/tools
)

file(GLOB_RECURSE SOURCES
    *.cpp
)

add_executable(${PROJECT_NAME}-tests
   ${SOURCES}
)

enable_testing()

add_dependencies(${PROJECT_NAME}-tests googletest)
target_link_libraries(${PROJECT_NAME}-tests gtest gmock gtest_main pthread ${PROJECT_NAME}-src)

add_test(
    NAME    ${PROJECT_NAME}-tests
    COMMAND ${PROJECT_NAME}-tests
                --gtest_output=xml:${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}-tests.xml
                --gtest_filter=${TESTCASES}
)
set_tests_properties(${PROJECT_NAME}-tests PROPERTIES LABELS TESTS)
