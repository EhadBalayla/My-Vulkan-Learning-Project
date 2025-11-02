#include "VertexBuffer.h"
#include "Engine.h"
#include <iostream>

#include "EngineUtilities.h"

void VertexBuffer::createVertexBuffer(const std::vector<Vertex>& verticies) {
	VkDeviceSize size = sizeof(Vertex) * verticies.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	RendUtils::createBuffer(stagingBuffer, stagingBufferMemory, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void* data;
	vkMapMemory(Engine::m_Renderer.getDevice(), stagingBufferMemory, 0, size, 0, &data);
	memcpy(data, verticies.data(), (size_t)size);
	vkUnmapMemory(Engine::m_Renderer.getDevice(), stagingBufferMemory);

	RendUtils::createBuffer(vertexBuffer, vertexBufferMemory, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	RendUtils::copyBuffer(stagingBuffer, vertexBuffer, size);

	vkDestroyBuffer(Engine::m_Renderer.getDevice(), stagingBuffer, nullptr);
	vkFreeMemory(Engine::m_Renderer.getDevice(), stagingBufferMemory, nullptr);
}
void VertexBuffer::bindBuffer() {
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(Engine::m_Renderer.getBufferAt(Engine::m_Renderer.currentFrame), 0, 1, &vertexBuffer, offsets);
}
void VertexBuffer::destroyBuffer() {
	vkDestroyBuffer(Engine::m_Renderer.getDevice(), vertexBuffer, nullptr);
	vkFreeMemory(Engine::m_Renderer.getDevice(), vertexBufferMemory, nullptr);
}