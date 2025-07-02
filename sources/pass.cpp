#include "pass.hpp"

#include "manager.hpp"
#include "graphics.hpp"
#include "utilities.hpp"

#include <stdexcept>

std::vector<VkAttachmentReference> PassConfig::GetColorReferences()
{
	std::vector<VkAttachmentReference> references;

	for (AttachmentConfig& attachment : colorAttachments) references.push_back(attachment.reference);

	return (references);
}

std::vector<VkAttachmentDescription> PassConfig::GetAttachments()
{
	std::vector<VkAttachmentDescription> attachments;

	for (AttachmentConfig& attachment : colorAttachments) attachments.push_back(attachment.description);

	if (depthAttachment.view) attachments.push_back(depthAttachment.description);

	return (attachments);
}

std::vector<VkImageView> PassConfig::GetViews()
{
	std::vector<VkImageView> views;

	for (AttachmentConfig& attachment : colorAttachments) views.push_back(attachment.view);

	if (depthAttachment.view) views.push_back(depthAttachment.view);

	return (views);
}

Pass::Pass()
{

}

Pass::~Pass()
{
	Destroy();
}

void Pass::Create(const PassConfig& passConfig, Device* passDevice = nullptr)
{
	config = passConfig;
	device = passDevice;

	if (!device) device = &Manager::GetDevice();

	CreateRenderPass();
	CreateFramebuffers();
}

void Pass::CreateRenderPass()
{
	if (renderpass) throw (std::runtime_error("Render pass already exists"));
	if (!device) throw (std::runtime_error("Pass has no device"));

	std::vector<VkAttachmentDescription> attachments = config.GetAttachments();

	VkRenderPassCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	createInfo.attachmentCount = CUI(attachments.size());
	createInfo.pAttachments = attachments.data();
	createInfo.subpassCount = CUI(config.subpasses.size());
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
		std::vector<VkImageView> views = config.GetViews();
		views[0] = swapchainViews[i];

		VkFramebufferCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.renderPass = renderpass;
		createInfo.attachmentCount = CUI(views.size());
		createInfo.pAttachments = views.data();
		createInfo.width = 400;
		createInfo.height = 400;
		createInfo.layers = 1;

		if (vkCreateFramebuffer(device->GetLogicalDevice(), &createInfo, nullptr, &framebuffers[i]) != VK_SUCCESS)
			throw (std::runtime_error("Failed to create framebuffer"));
	}
}

void Pass::Destroy()
{
	if (!device) return;

	for (int i = 0; i < framebuffers.size(); i++) 
		vkDestroyFramebuffer(device->GetLogicalDevice(), framebuffers[i], nullptr);
	framebuffers.clear();

	if (renderpass)
	{
		vkDestroyRenderPass(device->GetLogicalDevice(), renderpass, nullptr);
		renderpass = nullptr;
	}
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