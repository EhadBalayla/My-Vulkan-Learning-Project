#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "VertexStruct.h"

class VertexBuffer
{
public:
	void createVertexBuffer(const std::vector<Vertex>& verticies);
	void bindBuffer();
	void destroyBuffer();
private:
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
};

