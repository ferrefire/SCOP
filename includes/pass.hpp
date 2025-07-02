#pragma once

#include "device.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>

struct AttachmentConfig
{
	VkImageView view = nullptr;
	VkAttachmentDescription description{};
	VkAttachmentReference reference{};
};

struct PassConfig
{
	std::vector<AttachmentConfig> colorAttachments;
	AttachmentConfig depthAttachment{};
	std::vector<VkSubpassDescription> subpasses;

	std::vector<VkAttachmentReference> GetColorReferences(); 
	std::vector<VkAttachmentDescription> GetAttachments();
	std::vector<VkImageView> GetViews();
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

		void Create(const PassConfig& passConfig, Device* passDevice);

		void Destroy();

		VkRenderPass& GetRenderpass();

		static VkAttachmentDescription DefaultColorAttachment();
		static AttachmentConfig DefaultAttachmentConfig();
		static PassConfig DefaultConfig();
};