# src/CMakeLists.txt
# RCS Library Source Files

# Collect all source files in this directory
set(RCS_LIB_SOURCES
    GyroLib.cpp
    KeypadLib.cpp
    DigSensorLib.cpp
    RelayLib.cpp
    SocketConLib.cpp
)

# Create static library from source files
add_library(rcs_lib STATIC ${RCS_LIB_SOURCES})

# Include parent directory for headers
target_include_directories(rcs_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# Find required packages for the library
find_package(Threads REQUIRED)

# Check if running on Raspberry Pi
if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm" OR CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    set(IS_RPI TRUE)
    message(STATUS "Building RCS library for Raspberry Pi (${CMAKE_SYSTEM_PROCESSOR} architecture)")
else()
    set(IS_RPI FALSE)
    message(STATUS "Building RCS library for non-Pi system (${CMAKE_SYSTEM_PROCESSOR} architecture)")
endif()

# Link threads to the library
target_link_libraries(rcs_lib PUBLIC ${CMAKE_THREAD_LIBS_INIT})

# Add compile definitions for Pi
if(IS_RPI)
    target_compile_definitions(rcs_lib PUBLIC RASPBERRY_PI=1)
endif()

# Set library properties
set_target_properties(rcs_lib PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)

# Installation rules for the library
install(TARGETS rcs_lib
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

# Install headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/
    DESTINATION include/rcs
    FILES_MATCHING PATTERN "*.h"
)

# Print configuration info
message(STATUS "RCS Library Configuration:")
message(STATUS "  Source files: ${RCS_LIB_SOURCES}")
message(STATUS "  Is Raspberry Pi: ${IS_RPI}")
message(STATUS "  Threads library: ${CMAKE_THREAD_LIBS_INIT}") 