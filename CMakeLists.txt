cmake_minimum_required(VERSION 3.12)
project(VulkanProject)

# Collect all .cpp and .hpp files from the src directory
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")

# Set the include directories
include_directories(lib/glm)

# Set the Vulkan SDK path
set(VULKAN_SDK_PATH "C:/VulkanSDK/1.3.261.1")  # Replace with the actual version number

# Find Vulkan
find_package(Vulkan REQUIRED)

# Add the shaders directory to the source files (for IDEs that support it)
file(GLOB SHADERS shaders/*.spv)

# Set the executable
add_executable(VulkanProject ${SOURCES} ${SHADERS}
        src/tnk_pipeline.hpp)

# Add a custom target to compile shaders
add_custom_target(CompileShaders
        COMMAND cmd compile.bat
        COMMENT "Compiling shaders"
        VERBATIM
)
# Add CompileShaders as a dependency for VulkanProject
add_dependencies(VulkanProject CompileShaders)

# Add the GLFW subdirectory
add_subdirectory(lib/glfw3)

# Link libraries
target_link_libraries(VulkanProject
    Vulkan::Vulkan
    glfw
)

# Set compiler options
if (MSVC)
    target_compile_options(VulkanProject PRIVATE /W4)
else()
    target_compile_options(VulkanProject PRIVATE -Wall -Wextra -pedantic)
endif()

# Set output directories for binaries
set_target_properties(VulkanProject PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

# Copy shaders to build directory
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
