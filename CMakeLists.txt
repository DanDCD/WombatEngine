cmake_minimum_required(VERSION 3.5.0)

# Set project name and version
project(threedimsim VERSION 0.1.0 LANGUAGES C CXX)

# Make sure to export compile commands for Vscode C++ extension linter
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include CTest module and enable testing
include(CTest)
enable_testing()

# Set directories for dependencies using cache variables
set(GLFW_DIR "" CACHE PATH "Path to the GLFW library")
set(GLAD_DIR "" CACHE PATH "Path to the GLAD library")
set(GLM_DIR "" CACHE PATH "Path to the GLM library")
set(IMGUI_DIR "" CACHE PATH "Path to the IMGUI library")
set(ASSIMP_DIR "" CACHE PATH "Path to the ASSIMP library")

# Display status messages
message(STATUS "GLFW directory: ${GLFW_DIR}")
message(STATUS "GLAD directory: ${GLAD_DIR}")
message(STATUS "GLM directory: ${GLM_DIR}")
message(STATUS "IMGUI directory: ${IMGUI_DIR}")
message(STATUS "ASSIMP directory: ${ASSIMP_DIR}")
message(STATUS "Project name: ${PROJECT_NAME}")
message(STATUS "Project version: ${PROJECT_VERSION}")

# Function to add a library from a given directory
function(add_library_from_dir lib_name dir src_file)
    add_library(${lib_name} "${dir}/${src_file}")
    target_include_directories(${lib_name} PUBLIC "${dir}/include")
endfunction()

# Add GLFW
add_subdirectory(${GLFW_DIR} "binary_directory")

# Include GLAD
add_library_from_dir(glad ${GLAD_DIR} "src/glad.c")

# Include ImGui core source files
file(GLOB IMGUI_SOURCES "${IMGUI_DIR}/*.cpp")

# Include specific ImGui backend source files
set(IMGUI_BACKEND_SOURCES
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)

# Combine core and specific backend source files into a single library
add_library(imgui ${IMGUI_SOURCES} ${IMGUI_BACKEND_SOURCES})

# Add the DLL as an imported library
add_library(assimp SHARED IMPORTED)
set_target_properties(assimp PROPERTIES
    IMPORTED_LOCATION "${ASSIMP_DIR}/build/bin/libassimp-5d.dll"
    IMPORTED_IMPLIB "${ASSIMP_DIR}/build/lib/libassimpd.dll.a"
)

# Add executable
file(GLOB_RECURSE ALL_CPP_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(threedimsim ${ALL_CPP_FILES})

# Set include directories for threedimsim
target_include_directories(threedimsim PUBLIC 
    "${ASSIMP_DIR}/include" 
    "${ASSIMP_DIR}/build/include" 
    "${GLM_DIR}" 
    "${CMAKE_SOURCE_DIR}/includes"
)

# Link libraries to the executable
target_link_libraries(threedimsim glfw glad imgui assimp)

# Find and link OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(threedimsim OpenGL::GL)

# Set include directories for imgui
target_include_directories(imgui PUBLIC 
    ${IMGUI_DIR} 
    ${IMGUI_DIR}/backends 
    ${GLFW_DIR}/include
)

# Custom command to copy shaders directory
add_custom_command(TARGET threedimsim POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/shaders" $<TARGET_FILE_DIR:threedimsim>/shaders)

# Custom command to copy textures directory
add_custom_command(TARGET threedimsim POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/textures" $<TARGET_FILE_DIR:threedimsim>/textures)

# Custom command to copy models directory
add_custom_command(TARGET threedimsim POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/models" $<TARGET_FILE_DIR:threedimsim>/models)

# Ensure the DLL for assimp is copied to the output directory
add_custom_command(TARGET threedimsim POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${ASSIMP_DIR}/build/bin/libassimp-5d.dll"
        $<TARGET_FILE_DIR:threedimsim>)

# CPack settings for packaging the project
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
