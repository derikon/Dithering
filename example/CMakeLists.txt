# Set name for executable
set(EXECUTABLE_NAME ${PROJECT_NAME}_example)

# Create executable
add_executable(${EXECUTABLE_NAME} main.cpp)

# Link dependencies
target_link_libraries(${EXECUTABLE_NAME}
    ${PROJECT_NAME}
    opencv_highgui
)
