#include "pass.hpp"

#include "graphics.hpp"

#include <stdexcept>

Pass::Pass()
{

}

Pass::~Pass()
{

}

void Pass::Create(PassConfig config, Device* device)
{

}

void Pass::CreateRenderPass()
{
	if (renderpass) throw (std::runtime_error("Render pass already exists"));
	if (!device) throw (std::runtime_error("Pass has no device"));

	VkRenderPassCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	createInfo.attachmentCount = config.colorAttachments.size();
	createInfo.pAttachments = config.colorAttachments.data();
	createInfo.subpassCount = config.subpasses.size();
	createInfo.pSubpasses = config.subpasses.data();

	if (vkCreateRenderPass(device->GetLogicalDevice(), &createInfo, nullptr, &renderpass) != VK_SUCCESS)
		throw (std::runtime_error("Failed to create render pass"));
}

void Pass::CreateFramebuffers()
{
	if (framebuffers.size() != 0) throw (std::runtime_error("Frame buffers already exist"));
	if (!renderpass) throw (std::runtime_error("Render pass does not exist yet"));
	if (!device) throw (std::runtime_error("Pass has no device"));

	std::vector<VkImageView>& swapchainViews = Graphics::GetSwapchainViews();

	if (swapchainViews.size() == 0) throw (std::runtime_error("Swapchain has no views"));

	framebuffers.resize(Graphics::GetSwapchainViews().size());
	for (int i = 0; i < swapchainViews.size(); i++)
	{
		VkFramebufferCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.renderPass = renderpass;
		createInfo.attachmentCount = config.colorAttachments.size();
		createInfo.pAttachments = &swapchainViews[i];
		//continue form here
	}
}

void Pass::Destroy()
{

}

VkAttachmentDescription Pass::DefaultColorAttachment()
{
	VkAttachmentDescription description{};
	description.format = VK_FORMAT_B8G8R8A8_UNORM;
	description.samples = VK_SAMPLE_COUNT_1_BIT;
	description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	description.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	return (description);
}