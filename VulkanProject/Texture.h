#pragma once
#include "stbi_image.h"
#include <vulkan/vulkan.h>

class Texture
{
public:
	void LoadTexture(const char* path);
	void DestroyTexture();

	VkImageView& getImageView();
private:
	void createImageView();

	uint32_t mipLevels;
	VkImage image;
	VkDeviceMemory imageMem;
	VkImageView imageView;
};

