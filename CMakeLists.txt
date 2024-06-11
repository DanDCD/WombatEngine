cmake_minimum_required(VERSION 3.5.0)

# Set project name and version
project(threedimsim VERSION 0.1.0 LANGUAGES C CXX)

# Make sure to export compile commands for Vscode C++ extension linter
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include CTest module and enable testing
include(CTest)
enable_testing()

# Set directories for dependencies
#pc
# set(GLFW_DIR "C:/dev/cppsources/glfw-3.4")
# set(GLAD_DIR "C:/dev/cppsources/glad")
# set(GLM_DIR "C:/dev/cppsources/glm/glm")
#laptop
# set (GLFW_DIR "C:/Dev/Cpp/sources/glfw-3.4")
# set (GLAD_DIR "C:/Dev/Cpp/sources/glad")


# we can set these with ctrl+p cmake cache editor in vscode
# Set directories for dependencies using cache variables
set(GLFW_DIR "" CACHE PATH "Path to the GLFW library")
set(GLAD_DIR "" CACHE PATH "Path to the GLAD library")
set(GLM_DIR "" CACHE PATH "Path to the GLM library")


# Add GLFW
add_subdirectory(${GLFW_DIR} "binary_directory")

# Include GLAD
add_library(glad "${GLAD_DIR}/src/glad.c")
target_include_directories(glad PUBLIC "${GLAD_DIR}/include")

# Add executable
file(GLOB_RECURSE ALL_CPP_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(threedimsim ${ALL_CPP_FILES})

# Link libraries to the executable
target_link_libraries(threedimsim glfw glad)

# Set include directories for GLM
target_include_directories(threedimsim PUBLIC "${GLM_DIR}" "${CMAKE_SOURCE_DIR}/includes")

# Find and link OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(threedimsim OpenGL::GL)


# Custom command to copy shaders directory
add_custom_command(TARGET threedimsim POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/shaders" $<TARGET_FILE_DIR:threedimsim>/shaders)

# Custom command to copy textures directory
add_custom_command(TARGET threedimsim POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/textures" $<TARGET_FILE_DIR:threedimsim>/textures)


# CPack settings for packaging the project
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# Display status messages
message(STATUS "GLFW directory: ${GLFW_DIR}")
message(STATUS "GLAD directory: ${GLAD_DIR}")
message(STATUS "GLM directory: ${GLM_DIR}")
message(STATUS "Project name: ${PROJECT_NAME}")
message(STATUS "Project version: ${PROJECT_VERSION}")


