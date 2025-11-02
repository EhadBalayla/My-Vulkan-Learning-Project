set(VULKAN_SDK $ENV{VULKAN_SDK})

file(GLOB_RECURSE VKPROJ_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Headers/*.h"
)
add_executable(VulkanProject ${VKPROJ_SOURCES})

target_include_directories(VulkanProject PRIVATE
	${VULKAN_SDK}/Include
	${CMAKE_CURRENT_SOURCE_DIR}/Headers
	${CMAKE_CURRENT_SOURCE_DIR}/Includes
)

target_link_directories(VulkanProject PRIVATE
    ${VULKAN_SDK}/Lib
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries
)

target_link_libraries(VulkanProject PRIVATE
	assimp-vc143-mtd
    glfw3
	vulkan-1
)

set_target_properties(VulkanProject PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Compiled
	RUNTIME_OUTPUT_NAME "VulkanProject"
)