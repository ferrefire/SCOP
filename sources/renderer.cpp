#include "renderer.hpp"

#include "manager.hpp"

#include <stdexcept>

Renderer::Renderer()
{

}

Renderer::~Renderer()
{
	Destroy();
}

void Renderer::Create(uint32_t rendererFrameCount, Device* rendererDevice = nullptr, Swapchain* rendererSwapchain = nullptr)
{
	frameCount = rendererFrameCount;
	device = rendererDevice;
	swapchain = rendererSwapchain;

	if (frameCount <= 0) throw (std::runtime_error("Renderer frame count cannot be zero"));
	if (!device) device = &Manager::GetDevice();
	if (!swapchain) swapchain = &Manager::GetSwapchain();

	CreateFences();
	CreateSemaphores();
	CreateCommands();
}

void Renderer::CreateFences()
{
	if (!device) throw (std::runtime_error("Renderer has no device"));
	if (fences.size() != 0) throw (std::runtime_error("Renderer fences already exist"));

	fences.resize(frameCount);

	for (int i = 0; i < frameCount; i++)
	{
		VkFenceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		if (vkCreateFence(device->GetLogicalDevice(), &createInfo, nullptr, &fences[i]) != VK_SUCCESS)
			throw (std::runtime_error("Failed to create fence"));
	}
}

void Renderer::CreateSemaphores()
{
	if (!device) throw (std::runtime_error("Renderer has no device"));
	if (renderSemaphores.size() != 0) throw (std::runtime_error("Renderer render semaphores already exist"));
	if (presentSemaphores.size() != 0) throw (std::runtime_error("Renderer present semaphores already exist"));

	renderSemaphores.resize(frameCount);
	presentSemaphores.resize(frameCount);

	VkSemaphoreCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	for (int i = 0; i < frameCount; i++)
	{
		if (vkCreateSemaphore(device->GetLogicalDevice(), &createInfo, nullptr, &renderSemaphores[i]) != VK_SUCCESS)
			throw (std::runtime_error("Failed to create render semaphore"));
		if (vkCreateSemaphore(device->GetLogicalDevice(), &createInfo, nullptr, &presentSemaphores[i]) != VK_SUCCESS)
			throw (std::runtime_error("Failed to create present semaphore"));
	}
}

void Renderer::CreateCommands()
{
	if (!device) throw (std::runtime_error("Renderer has no device"));
	if (commands.size() != 0) throw (std::runtime_error("Renderer commands already exist"));
	if (fences.size() == 0) throw (std::runtime_error("Renderer fences do not exist"));
	if (renderSemaphores.size() == 0) throw (std::runtime_error("Renderer render semaphores do not exist"));
	if (presentSemaphores.size() == 0) throw (std::runtime_error("Renderer present semaphores do not exist"));

	commands.resize(frameCount);

	for (int i = 0; i < frameCount; i++)
	{
		CommandConfig commandConfig{};
		commandConfig.queueIndex = device->GetQueueIndex(QueueType::Graphics);
		commandConfig.wait = false;
		commandConfig.fence = fences[i];
		commandConfig.waitSemaphores = {renderSemaphores[i]};
		commandConfig.waitDestinations = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		commandConfig.signalSemaphores = {presentSemaphores[i]};

		commands[i].Create(commandConfig, device);
	}
}

void Renderer::Destroy()
{
	if (!device) return;

	for (VkFence& fence : fences)
	{
		vkDestroyFence(device->GetLogicalDevice(), fence, nullptr);
	}
	fences.clear();

	for (VkSemaphore& semaphore : renderSemaphores)
	{
		vkDestroySemaphore(device->GetLogicalDevice(), semaphore, nullptr);
	}
	renderSemaphores.clear();

	for (VkSemaphore& semaphore : presentSemaphores)
	{
		vkDestroySemaphore(device->GetLogicalDevice(), semaphore, nullptr);
	}
	presentSemaphores.clear();

	for (Command& command : commands)
	{
		command.Destroy();
	}
	commands.clear();
}

void Renderer::Frame()
{
	if (vkWaitForFences(device->GetLogicalDevice(), 1, &fences[currentFrame], VK_TRUE, UINT64_MAX) != VK_SUCCESS)
		throw (std::runtime_error("Failed to wait for fence"));

	if (vkResetFences(device->GetLogicalDevice(), 1, &fences[currentFrame]) != VK_SUCCESS)
		throw (std::runtime_error("Failed to reset fence"));

	if (vkAcquireNextImageKHR(device->GetLogicalDevice(), swapchain->GetSwapchain(), UINT64_MAX, 
		renderSemaphores[currentFrame], VK_NULL_HANDLE, &renderIndex) != VK_SUCCESS)
			throw (std::runtime_error("Failed to aquire next render image index"));

	RecordCommands();
	PresentFrame();

	currentFrame = (currentFrame + 1) % frameCount;
}

void Renderer::RecordCommands()
{
	commands[currentFrame].Begin();

	for (Pass* pass : passes)
	{
		pass->Begin(commands[currentFrame].GetBuffer(), renderIndex);

		for (std::function<void(VkCommandBuffer, uint32_t)> call : calls)
		{
			call(commands[currentFrame].GetBuffer(), currentFrame);
		}

		pass->End(commands[currentFrame].GetBuffer());
	}

	/*VkImageMemoryBarrier renderBarrier{};
	renderBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	renderBarrier.srcAccessMask = 0;
	renderBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	renderBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	renderBarrier.newLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR;
	renderBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	renderBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	renderBarrier.image = swapchain->GetImages()[renderIndex];
	renderBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	renderBarrier.subresourceRange.baseMipLevel = 0;
	renderBarrier.subresourceRange.levelCount = 1;
	renderBarrier.subresourceRange.baseArrayLayer = 0;
	renderBarrier.subresourceRange.layerCount = 1;

	vkCmdPipelineBarrier(commands[currentFrame].GetBuffer(), VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0, nullptr, 1, &renderBarrier);

	VkRenderingAttachmentInfo attachmentInfo{};
	attachmentInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
	attachmentInfo.imageView = swapchain->GetViews()[renderIndex];
	attachmentInfo.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR;
	attachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachmentInfo.clearValue = {.color = {{0.0f, 0.0f, 0.0f, 0.0f}}};

	VkRenderingInfo renderingInfo{};
	renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
	renderingInfo.renderArea.offset = {0, 0};
	renderingInfo.renderArea.extent = Manager::GetWindow().GetConfig().extent;
	renderingInfo.layerCount = 1;
	renderingInfo.colorAttachmentCount = 1;
	renderingInfo.pColorAttachments = &attachmentInfo;

	vkCmdBeginRendering(commands[currentFrame].GetBuffer(), &renderingInfo);

	for (std::function<void(VkCommandBuffer, uint32_t)> call : calls)
	{
		call(commands[currentFrame].GetBuffer(), currentFrame);
	}

	vkCmdEndRendering(commands[currentFrame].GetBuffer());

	VkImageMemoryBarrier presentBarrier{};
	presentBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	presentBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	presentBarrier.dstAccessMask = 0;
	presentBarrier.oldLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR;
	presentBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	presentBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	presentBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	presentBarrier.image = swapchain->GetImages()[renderIndex];
	presentBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	presentBarrier.subresourceRange.baseMipLevel = 0;
	presentBarrier.subresourceRange.levelCount = 1;
	presentBarrier.subresourceRange.baseArrayLayer = 0;
	presentBarrier.subresourceRange.layerCount = 1;

	vkCmdPipelineBarrier(commands[currentFrame].GetBuffer(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 
		VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &presentBarrier);*/

	commands[currentFrame].End();

	commands[currentFrame].Submit();
}

void Renderer::PresentFrame()
{
	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &presentSemaphores[currentFrame];
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapchain->GetSwapchain();
	presentInfo.pImageIndices = &renderIndex;

	if (vkQueuePresentKHR(device->GetQueue(device->GetQueueIndex(QueueType::Graphics)), &presentInfo) != VK_SUCCESS)
		throw (std::runtime_error("Failed to present frame"));
}

void Renderer::AddPass(Pass* pass)
{
	if (!pass) throw (std::runtime_error("Cannot add pass because it does not exist"));

	passes.push_back(pass);
}

void Renderer::RegisterCall(std::function<void(VkCommandBuffer, uint32_t)> call)
{
	calls.push_back(call);
}

Device* Renderer::device = nullptr;
Swapchain* Renderer::swapchain = nullptr;

uint32_t Renderer::frameCount = 0;
uint32_t Renderer::currentFrame = 0;
uint32_t Renderer::renderIndex = 0;

std::vector<VkFence> Renderer::fences;
std::vector<VkSemaphore> Renderer::renderSemaphores;
std::vector<VkSemaphore> Renderer::presentSemaphores;
std::vector<Command> Renderer::commands;
std::vector<Pass*> Renderer::passes;
std::vector<std::function<void(VkCommandBuffer, uint32_t)>> Renderer::calls;