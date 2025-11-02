#pragma once
#include <vulkan/vulkan.h>
#include <vector>

class IndexBuffer
{
public:
	void createIndexBuffer(const std::vector<uint32_t>& indicies);
	void bindBuffer();
	void destroyBuffer();
private:
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
};

