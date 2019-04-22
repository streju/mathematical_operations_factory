cmake_minimum_required(VERSION 2.8)

project(mathematical_operations_factory)
add_executable(${PROJECT_NAME} "main.cpp"
    "Manager.cpp"
    "Worker.cpp"
    "MachinesService.cpp"
    "AdditionMachine.cpp"
    "SubstractionMachine.cpp"
    "MultiplicationMachine.cpp"
    "DivisionMachine.cpp"
    "Machines.hpp"   #not used
    "src/WorkersPool.hpp"
    "src/ThreadWrapper.hpp"
    "src/Tools/Logger.cpp")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set_property(TARGET mathematical_operations_factory PROPERTY CXX_STANDARD 17)
