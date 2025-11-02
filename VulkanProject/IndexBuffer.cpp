#include "IndexBuffer.h"
#include "Engine.h"
#include <iostream>

#include "EngineUtilities.h"

void IndexBuffer::createIndexBuffer(const std::vector<uint32_t>& indicies) {
	VkDeviceSize bufferSize = sizeof(indicies[0]) * indicies.size();
	
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	RendUtils::createBuffer(stagingBuffer, stagingBufferMemory, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void* data;
	vkMapMemory(Engine::m_Renderer.getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indicies.data(), (size_t)bufferSize);
	vkUnmapMemory(Engine::m_Renderer.getDevice(), stagingBufferMemory);

	RendUtils::createBuffer(indexBuffer, indexBufferMemory, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	RendUtils::copyBuffer(stagingBuffer, indexBuffer, bufferSize);

	vkDestroyBuffer(Engine::m_Renderer.getDevice(), stagingBuffer, nullptr);
	vkFreeMemory(Engine::m_Renderer.getDevice(), stagingBufferMemory, nullptr);
}
void IndexBuffer::bindBuffer() {
	vkCmdBindIndexBuffer(Engine::m_Renderer.getBufferAt(Engine::m_Renderer.currentFrame), indexBuffer, 0, VK_INDEX_TYPE_UINT32);
}
void IndexBuffer::destroyBuffer() {
	vkDestroyBuffer(Engine::m_Renderer.getDevice(), indexBuffer, nullptr);
	vkFreeMemory(Engine::m_Renderer.getDevice(), indexBufferMemory, nullptr);
}