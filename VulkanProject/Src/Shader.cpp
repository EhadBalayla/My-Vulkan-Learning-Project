#include "Shader.h"
#include <fstream>
#include "Engine.h"
#include "VertexBuffer.h"
#include "EngineUtilities.h"
#include <glm/gtc/matrix_transform.hpp>

void Shader::LoadTexture(const char* path) {
	texture1 = new Texture();
	texture1->LoadTexture(path);
}

void Shader::UnloadTexture() {
	texture1->DestroyTexture();
}

void Shader::LoadShader(const std::string& vertexPath, const std::string& fragmentPath) {
	createUniformBuffers();
	createDescriptorSetLayout();
	createDescriptorSets();

	//loads in the shader files
	auto vertexShaderCode = readShaderFile(vertexPath);
	auto fragmentShaderCode = readShaderFile(fragmentPath);

	VkShaderModule vertexModule = createShaderModule(vertexShaderCode);
	VkShaderModule fragmentModule = createShaderModule(fragmentShaderCode);

	//create the shader stages
	VkPipelineShaderStageCreateInfo vertexStageInfo{};
	vertexStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertexStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertexStageInfo.module = vertexModule;
	vertexStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragmentStageInfo{};
	fragmentStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragmentStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragmentStageInfo.module = fragmentModule;
	fragmentStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertexStageInfo, fragmentStageInfo };

	//creation of the vertex inputs
	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	//creation of the input assembly
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
	inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	//creation of the viewport state
	VkPipelineViewportStateCreateInfo viewportInfo{};
	viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportInfo.viewportCount = 1;
	viewportInfo.scissorCount = 1;

	//creation of the dynamic state
	std::vector<VkDynamicState> dynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};
	VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
	dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicStateInfo.pDynamicStates = dynamicStates.data();

	//creation of the rasterizer
	VkPipelineRasterizationStateCreateInfo rasterizerInfo{};
	rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizerInfo.depthClampEnable = VK_FALSE;
	rasterizerInfo.rasterizerDiscardEnable = VK_FALSE;
	rasterizerInfo.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizerInfo.lineWidth = 1.0f;
	rasterizerInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizerInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizerInfo.depthBiasEnable = VK_FALSE;

	//creation of the multisampling
	VkPipelineMultisampleStateCreateInfo multisamplingInfo{};
	multisamplingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisamplingInfo.sampleShadingEnable = VK_FALSE;
	multisamplingInfo.rasterizationSamples = Engine::m_Renderer.getSampleCount();
	multisamplingInfo.sampleShadingEnable = VK_TRUE;
	multisamplingInfo.minSampleShading = 0.2f;

	//creation of the color blending
	VkPipelineColorBlendAttachmentState colorAttachmentInfo{};
	colorAttachmentInfo.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorAttachmentInfo.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlendingInfo{};
	colorBlendingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendingInfo.logicOpEnable = VK_FALSE;
	colorBlendingInfo.attachmentCount = 1;
	colorBlendingInfo.pAttachments = &colorAttachmentInfo;

	//creation of the depth testing
	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f; // Optional
	depthStencil.maxDepthBounds = 1.0f; // Optional
	depthStencil.stencilTestEnable = VK_FALSE;
	depthStencil.front = {}; // Optional
	depthStencil.back = {}; // Optional

	//creation of the pipeline layout
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
	pipelineLayoutInfo.setLayoutCount = 1;

	if (vkCreatePipelineLayout(Engine::m_Renderer.getDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("couldn't make the pipeline layout for shader");
	}

	//creation of the graphics pipeline
	VkGraphicsPipelineCreateInfo graphicsPipelineInfo{};
	graphicsPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	graphicsPipelineInfo.stageCount = 2;
	graphicsPipelineInfo.pStages = shaderStages;
	graphicsPipelineInfo.pVertexInputState = &vertexInputInfo;
	graphicsPipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
	graphicsPipelineInfo.pViewportState = &viewportInfo;
	graphicsPipelineInfo.pRasterizationState = &rasterizerInfo;
	graphicsPipelineInfo.pMultisampleState = &multisamplingInfo;
	graphicsPipelineInfo.pColorBlendState = &colorBlendingInfo;
	graphicsPipelineInfo.pDynamicState = &dynamicStateInfo;
	graphicsPipelineInfo.pDepthStencilState = &depthStencil;
	graphicsPipelineInfo.layout = pipelineLayout;
	graphicsPipelineInfo.renderPass = Engine::m_Renderer.getRenderPass();
	graphicsPipelineInfo.subpass = 0;
	graphicsPipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	graphicsPipelineInfo.basePipelineIndex = -1;

	if (vkCreateGraphicsPipelines(Engine::m_Renderer.getDevice(), VK_NULL_HANDLE, 1, &graphicsPipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics pipeline for shader");
	}

	vkDestroyShaderModule(Engine::m_Renderer.getDevice(), vertexModule, nullptr);
	vkDestroyShaderModule(Engine::m_Renderer.getDevice(), fragmentModule, nullptr);
}

void Shader::UnloadShader() {
	for (int i = 0; i < Engine::m_Renderer.MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroyBuffer(Engine::m_Renderer.getDevice(), uniformBuffers[i], nullptr);
		vkFreeMemory(Engine::m_Renderer.getDevice(), uniformBuffersMemory[i], nullptr);
	}

	vkDestroyDescriptorSetLayout(Engine::m_Renderer.getDevice(), descriptorSetLayout, nullptr);

	vkDestroyPipeline(Engine::m_Renderer.getDevice(), graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(Engine::m_Renderer.getDevice(), pipelineLayout, nullptr);
}

void Shader::Bind(VkCommandBuffer commandBuffer) {
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(Engine::m_Renderer.getSwapchainExtent().width);
	viewport.height = static_cast<float>(Engine::m_Renderer.getSwapchainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = Engine::m_Renderer.getSwapchainExtent();
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[Engine::m_Renderer.currentFrame], 0, nullptr);
}

void Shader::updateUniformBuffers(glm::mat4 transformMatrix) {
	UniformBufferObject datUBO;

	datUBO.proj = glm::perspective(glm::radians(45.0f), Engine::m_Renderer.getSwapchainExtent().width / (float)Engine::m_Renderer.getSwapchainExtent().height, 0.1f, 100.0f);
	datUBO.view = Engine::cam.GetViewMatrix();
	datUBO.model = transformMatrix;
	datUBO.camPos = Engine::cam.Position;

	memcpy(uniformBuffersMapped[Engine::m_Renderer.currentFrame], &datUBO, sizeof(datUBO));
}

std::vector<char> Shader::readShaderFile(const std::string& path) {
	std::ifstream file(path.c_str(), std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("couldn't open file");
	}

	size_t fileSize = static_cast<size_t>(file.tellg());
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

VkShaderModule Shader::createShaderModule(const std::vector<char>& shaderCode) {
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = shaderCode.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(Engine::m_Renderer.getDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module");
	}

	return shaderModule;
}

void Shader::createDescriptorSetLayout() {
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;


	VkDescriptorSetLayoutBinding texBinding{};
	texBinding.binding = 1;
	texBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	texBinding.descriptorCount = 1;
	texBinding.pImmutableSamplers = nullptr;
	texBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;


	std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, texBinding };
	VkDescriptorSetLayoutCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	createInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	createInfo.pBindings = bindings.data();



	if (vkCreateDescriptorSetLayout(Engine::m_Renderer.getDevice(), &createInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout for shader");
	}
}

void Shader::createDescriptorSets() {
	std::vector<VkDescriptorSetLayout> layouts(Engine::m_Renderer.MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = Engine::m_Renderer.getDescriptorPool();
	allocInfo.descriptorSetCount = static_cast<uint32_t>(Engine::m_Renderer.MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(Engine::m_Renderer.MAX_FRAMES_IN_FLIGHT);
	if (vkAllocateDescriptorSets(Engine::m_Renderer.getDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets for shader");
	}

	for (size_t i = 0; i < Engine::m_Renderer.MAX_FRAMES_IN_FLIGHT; i++) {
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = texture1->getImageView();
		imageInfo.sampler = Engine::m_Renderer.getSampler();

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSets[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;

		VkWriteDescriptorSet descriptorWriteTwo{};
		descriptorWriteTwo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWriteTwo.dstSet = descriptorSets[i];
		descriptorWriteTwo.dstBinding = 1;
		descriptorWriteTwo.dstArrayElement = 0;
		descriptorWriteTwo.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWriteTwo.descriptorCount = 1;
		descriptorWriteTwo.pImageInfo = &imageInfo;

		std::array<VkWriteDescriptorSet, 2> descriptorWrites{ descriptorWrite , descriptorWriteTwo };

		vkUpdateDescriptorSets(Engine::m_Renderer.getDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void Shader::createUniformBuffers() {
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	uniformBuffers.resize((size_t)Engine::m_Renderer.MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMemory.resize((size_t)Engine::m_Renderer.MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMapped.resize((size_t)Engine::m_Renderer.MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < Engine::m_Renderer.MAX_FRAMES_IN_FLIGHT; i++) {
		RendUtils::createBuffer(uniformBuffers[i], uniformBuffersMemory[i], bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		vkMapMemory(Engine::m_Renderer.getDevice(), uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
	}
}