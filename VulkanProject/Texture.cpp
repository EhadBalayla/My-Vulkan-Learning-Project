#include "Texture.h"
#include <stdexcept>
#include "EngineUtilities.h"
#include "Engine.h"

void Texture::LoadTexture(const char* path) {
	int width, height, channels;
	unsigned char* pixel_data = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);

	VkDeviceSize imageSize = width * height * 4;

	if (!pixel_data) {
		throw std::runtime_error("couldn't load in the texture");
	}

	mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;


	VkBuffer stageBuffer;
	VkDeviceMemory stageBufferMemory;

	RendUtils::createBuffer(stageBuffer, stageBufferMemory, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void* data;
	vkMapMemory(Engine::m_Renderer.getDevice(), stageBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, pixel_data, static_cast<uint32_t>(imageSize));
	vkUnmapMemory(Engine::m_Renderer.getDevice(), stageBufferMemory);

	stbi_image_free(pixel_data);


	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = static_cast<uint32_t>(width);
	imageInfo.extent.height = static_cast<uint32_t>(height);
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = mipLevels;
	imageInfo.arrayLayers = 1;
	imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;

	if (vkCreateImage(Engine::m_Renderer.getDevice(), &imageInfo, nullptr, &image) != VK_SUCCESS) {
		throw std::runtime_error("failed to create the image");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(Engine::m_Renderer.getDevice(), image, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = RendUtils::findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	if (vkAllocateMemory(Engine::m_Renderer.getDevice(), &allocInfo, nullptr, &imageMem) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate memory for this image");
	}

	vkBindImageMemory(Engine::m_Renderer.getDevice(), image, imageMem, 0);

	RendUtils::transitionImageLayout(image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevels);
	RendUtils::copyBufferToImage(stageBuffer, image, static_cast<uint32_t>(width), static_cast<uint32_t>(height));

	RendUtils::GenerateMipmaps(image, VK_FORMAT_R8G8B8A8_SRGB, width, height, mipLevels);

	vkDestroyBuffer(Engine::m_Renderer.getDevice(), stageBuffer, nullptr);
	vkFreeMemory(Engine::m_Renderer.getDevice(), stageBufferMemory, nullptr);

	createImageView();

}
void Texture::DestroyTexture() {
	vkDestroyImageView(Engine::m_Renderer.getDevice(), imageView, nullptr);

	vkDestroyImage(Engine::m_Renderer.getDevice(), image, nullptr);
	vkFreeMemory(Engine::m_Renderer.getDevice(), imageMem, nullptr);
}
void Texture::createImageView() {
	imageView = RendUtils::createImageView(image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, mipLevels);
}
VkImageView& Texture::getImageView() {
	return imageView;
}