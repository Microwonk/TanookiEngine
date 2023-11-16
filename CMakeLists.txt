cmake_minimum_required(VERSION 3.12)

set(EngineName "TanookiEngine")

project(${EngineName})

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

# Remove existing shader assets in build directory for CLEAN
file(GLOB EXISTING_SHADERS ${CMAKE_BINARY_DIR}/shaders/*.spv)
if(EXISTING_SHADERS)
    message(STATUS "Removing existing shaders in the build directory...")
    file(REMOVE ${EXISTING_SHADERS})
endif()

# Set the executable
add_executable(${EngineName} ${SOURCES} ${SHADERS})

# Add a custom target to compile shaders
add_custom_target(CompileShaders
        COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_SOURCE_DIR} cmd /c compile.bat
        COMMENT "Compiling shaders"
        VERBATIM
)

# Add CompileShaders as a dependency for VulkanProject
add_dependencies(${EngineName} CompileShaders)

# Copy only .spv files to build directory after compilation
file(GLOB_RECURSE SPV_FILES ${CMAKE_SOURCE_DIR}/shaders/*.spv)

add_custom_command(TARGET CompileShaders POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/shaders
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SPV_FILES} ${CMAKE_BINARY_DIR}/shaders
        COMMENT "Copying .spv files to build directory"
)



# Add the GLFW subdirectory
add_subdirectory(lib/glfw3)

# Link libraries
target_link_libraries(${EngineName}
    Vulkan::Vulkan
    glfw
)

# Set compiler options
if (MSVC)
    target_compile_options(${EngineName} PRIVATE /W4)
else()
    target_compile_options(${EngineName} PRIVATE -Wall -Wextra -pedantic)
endif()

# Set output directories for binaries
set_target_properties(${EngineName} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)
