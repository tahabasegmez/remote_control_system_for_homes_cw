cmake_minimum_required(VERSION 3.10)
project(RemoteControlSystem)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directory
include_directories(include)

# Add src subdirectory for library building
add_subdirectory(src)

# Check if we're on ARM (Raspberry Pi) - for main project info
if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm.*|ARM.*|aarch64.*|AARCH64.*")
    set(IS_RASPBERRY_PI TRUE)
    message(STATUS "Building main project for Raspberry Pi (ARM architecture detected)")
else()
    set(IS_RASPBERRY_PI FALSE)
    message(STATUS "Building main project for non-Pi system (x86/x64 architecture)")
endif()

# Check for i2c-dev (informational)
find_path(I2C_INCLUDE_DIR NAMES linux/i2c-dev.h)
if(NOT I2C_INCLUDE_DIR)
    message(WARNING "linux/i2c-dev.h not found. Gyro sensor may not work on some systems.")
endif()

# Create executables (Node files are in root rcs directory)
add_executable(DigitalIONode DigitalIONode.cpp)
add_executable(GyroSensorNode GyroSensorNode.cpp)
add_executable(ServerNode ServerNode.cpp)
add_executable(ClientNode ClientNode.cpp)

# Link executables with the library (rcs_lib is built in src/)
target_link_libraries(DigitalIONode rcs_lib)
target_link_libraries(GyroSensorNode rcs_lib)
target_link_libraries(ServerNode rcs_lib)
target_link_libraries(ClientNode rcs_lib)

# Set output directory for executables
set_target_properties(DigitalIONode GyroSensorNode ServerNode ClientNode
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Install targets
install(TARGETS DigitalIONode GyroSensorNode ServerNode ClientNode
    RUNTIME DESTINATION bin
)

# Print main project configuration info
message(STATUS "=== Remote Control System Main Project ===")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "System processor: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "Is Raspberry Pi: ${IS_RASPBERRY_PI}")
message(STATUS "I2C include dir: ${I2C_INCLUDE_DIR}")

# Add compile definitions for debug mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
    message(STATUS "Debug mode enabled")
endif()

# Print build targets
message(STATUS "Build targets:")
message(STATUS "  Library: rcs_lib (in src/)")
message(STATUS "  Executables: DigitalIONode, GyroSensorNode, ServerNode, ClientNode")
message(STATUS "  Output directories:")
message(STATUS "    Binaries: ${CMAKE_BINARY_DIR}/bin")
message(STATUS "    Libraries: ${CMAKE_BINARY_DIR}/lib") 