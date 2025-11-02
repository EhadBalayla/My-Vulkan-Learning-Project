#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Texture.h"

class Shader
{
public:
	void LoadTexture(const char* path);
	void UnloadTexture();
	void LoadShader(const std::string& vertexPath, const std::string& fragmentPath);
	void UnloadShader();
	void Bind(VkCommandBuffer commandBuffer);

	void updateUniformBuffers(glm::mat4 transformMatrix);
private:
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	VkDescriptorSetLayout descriptorSetLayout;
	std::vector<VkDescriptorSet> descriptorSets;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	std::vector<char> readShaderFile(const std::string& path);
	VkShaderModule createShaderModule(const std::vector<char>& shaderCode);
	void createDescriptorSetLayout();
	void createDescriptorSets();
	void createUniformBuffers();

	Texture* texture1;
};

