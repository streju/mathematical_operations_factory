cmake_minimum_required(VERSION 2.8)

project(mathematical_operations_factory)
add_executable(${PROJECT_NAME} "main.cpp" "Manager.cpp" "Worker.cpp" "OperationMachine.cpp")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set_property(TARGET mathematical_operations_factory PROPERTY CXX_STANDARD 17)
