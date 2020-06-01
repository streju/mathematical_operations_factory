add_library(${PROJECT_NAME}-src OBJECT
    "Manager.cpp"
    "Worker.cpp"
    "MachinesService.cpp"
    "Tools/Logger.cpp"
    "Warehouse.cpp"
    "WarehouseEntryPoint.cpp"
    "Transport.cpp"
    "Shop.cpp"
    "Customer.cpp"
    "Utils.cpp")

add_executable(${PROJECT_NAME} "main.cpp"
   $<TARGET_OBJECTS:${PROJECT_NAME}-src>)
