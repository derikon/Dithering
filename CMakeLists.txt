cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# Create project
project(dither VERSION 0.1.0 DESCRIPTION "a simple dithering lib" LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Find dependencies
find_package(OpenCV REQUIRED)

# Include header files
include_directories(include)

# Create shared library
file(GLOB SOURCES src/*.cpp)
add_library(${PROJECT_NAME} SHARED ${SOURCES})


set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

target_include_directories (${PROJECT_NAME} INTERFACE include)

# Link dependencies
target_link_libraries(${PROJECT_NAME}
    opencv_core
    opencv_imgproc
)

# Install library
install(TARGETS ${PROJECT_NAME} DESTINATION lib/${PROJECT_NAME})

# Install library headers
file(GLOB HEADERS include/*.hpp)
install(FILES ${HEADERS} DESTINATION include/${PROJECT_NAME})

# Build example by default
option(WITH_EXAMPLE "Build ${PROJECT_NAME} with example" ON)
if(WITH_EXAMPLE)
    message("Build ${PROJECT_NAME} with example")
    add_subdirectory(example)
endif(WITH_EXAMPLE)
