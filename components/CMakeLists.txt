# Components library for ESP-IDF

# Check component directory structure
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src)
    message(STATUS "Creating components source directory")
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)
endif()

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/include)
    message(STATUS "Creating components include directory")
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include)
endif()

# Components source files (if any)
file(GLOB COMPONENT_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Check if there are source files
if(NOT COMPONENT_SRCS)
    message(STATUS "No component source files found, creating empty library")
    # Create an empty source file to avoid CMake errors
    set(EMPTY_SOURCE ${CMAKE_CURRENT_BINARY_DIR}/empty.cpp)
    file(WRITE ${EMPTY_SOURCE} "// Empty file to avoid CMake errors\n")
    set(COMPONENT_SRCS ${EMPTY_SOURCE})
endif()

# Register the component using ESP-IDF component system
idf_component_register(
    SRCS ${COMPONENT_SRCS}
    INCLUDE_DIRS "include"
    PRIV_INCLUDE_DIRS "src"
    # Add any required components
    # REQUIRES framework_core
)