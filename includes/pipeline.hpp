#pragma once

#include "device.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

struct PipelineConfig
{
	VkPipelineViewportStateCreateInfo viewportState{};
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    VkPipelineRasterizationStateCreateInfo rasterization{};
    VkPipelineMultisampleStateCreateInfo multisampling{};
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    VkPipelineColorBlendStateCreateInfo colorBlending{};
    VkPipelineDepthStencilStateCreateInfo depthStencil{};

	std::string shader = "";

	VkRenderPass renderpass = nullptr;
	uint32_t subpass = 0;
};

class Pipeline
{
	private:
		PipelineConfig config{};
		Device* device = nullptr;

		VkPipelineLayout layout = nullptr;

		VkShaderModule CreateShader(const std::vector<char>& code);
		void CreateLayout();

	public:
		Pipeline();
		~Pipeline();

		void Create(const PipelineConfig& pipelineConfig, Device* pipelineDevice);

		void Destroy();

		static PipelineConfig DefaultConfig();
};