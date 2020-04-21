cmake_minimum_required(VERSION 2.8)

project(mathematical_operations_factory)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fdiagnostics-color=auto -Wno-deprecated -Wall -Werror -ggdb -Wextra -Wno-unused-parameter -pthread -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated -Wall -Werror -ggdb -Wextra -Wno-unused-parameter -fPIC")
#set_property(TARGET mathematical_operations_factory PROPERTY CXX_STANDARD 17)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
)

add_subdirectory(src)
add_subdirectory(tests)
