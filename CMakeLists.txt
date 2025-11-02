cmake_minimum_required(VERSION 3.20)
project(My-Vulkan-Learning-Proj LANGUAGES CXX)

# Default to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable folders in IDEs (Visual Studio, CLion, etc.)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Enable position independent code (needed for shared libs)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Add subprojects
add_subdirectory(VulkanProject)