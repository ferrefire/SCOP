#pragma once

#include "device.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

struct PassConfig
{
	std::vector<VkAttachmentDescription> colorAttachments;
	std::vector<VkAttachmentReference> colorReferences;
	std::vector<VkAttachmentDescription> depthAttachments;
	std::vector<VkAttachmentReference> depthReferences;
	std::vector<VkSubpassDescription> subpasses;
};

class Pass
{
	private:
		PassConfig config{};
		Device* device = nullptr;

		VkRenderPass renderpass = nullptr;
		std::vector<VkFramebuffer> framebuffers;

		void CreateRenderPass();
		void CreateFramebuffers();

	public:
		Pass();
		~Pass();

		void Create(PassConfig config, Device* device);

		void Destroy();

		static VkAttachmentDescription DefaultColorAttachment();
};