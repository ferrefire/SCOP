#include "manager.hpp"

#include "graphics.hpp"
#include "renderer.hpp"
#include "descriptor.hpp"
#include "utilities.hpp"
#include "input.hpp"

#include "pipeline.hpp"
#include "mesh.hpp"
#include "buffer.hpp"
#include "pass.hpp"
#include "bitmask.hpp"
#include "command.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <iostream>
#include <utility>

Manager::Manager()
{

}

Manager::~Manager()
{
	Destroy();
}

void Test(VkCommandBuffer commandBuffer, uint32_t currentFrame)
{
	static bool start = true;
	static Mesh<Position, VK_INDEX_TYPE_UINT16> mesh;
	static Pass pass;
	static Pipeline pipeline;
	static point3D transformation = point3D({-0.5f, 0.0f, 1.0f});
	static Buffer buffer;
	static Descriptor descriptor;

	if (start)
	{
		Vertex<Position> vert;
		vert.position = point3D({-0.5f, -0.5f, 0.0f});
		mesh.AddVertex(vert);
		vert.position = point3D({0.5f, 0.5f, 0.0f});
		mesh.AddVertex(vert);
		vert.position = point3D({-0.5f, 0.5f, 0.0f});
		mesh.AddVertex(vert);
		vert.position = point3D({0.5f, -0.5f, 0.0f});
		mesh.AddVertex(vert);

		mesh.AddIndex(0);
		mesh.AddIndex(1);
		mesh.AddIndex(2);
		mesh.AddIndex(3);
		mesh.AddIndex(1);
		mesh.AddIndex(0);

		mesh.Create(&Manager::GetDevice());

		PassConfig passConfig = Pass::DefaultConfig();
		pass.Create(passConfig, &Manager::GetDevice());

		BufferConfig bufferConfig{};
		bufferConfig.mapped = true;
		bufferConfig.size = sizeof(point3D);
		buffer.Create(bufferConfig, &Manager::GetDevice(), &transformation[0]);

		std::vector<DescriptorConfig> descriptorConfigs(1);
		descriptorConfigs[0].stages = VK_SHADER_STAGE_FRAGMENT_BIT;
		descriptor.Create(descriptorConfigs, &Manager::GetDevice());
		
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = buffer.GetBuffer();
		bufferInfo.range = sizeof(point3D);
		descriptor.Update(0, &bufferInfo, nullptr);

		PipelineConfig pipelineConfig = Pipeline::DefaultConfig();
		pipelineConfig.shader = "screen";
		pipelineConfig.vertexInfo = mesh.GetVertexInfo();
		pipelineConfig.renderpass = pass.GetRenderpass();
		pipelineConfig.rasterization.cullMode = VK_CULL_MODE_NONE;
		pipelineConfig.descriptorLayouts = {descriptor.GetLayout()};
		pipeline.Create(pipelineConfig, &Manager::GetDevice());

		Renderer::AddPass(&pass);

		start = false;
	}
	else if (commandBuffer)
	{
		if (Input::GetKey(GLFW_KEY_W).down) transformation.y() -= 0.01f / transformation.z();
		if (Input::GetKey(GLFW_KEY_S).down) transformation.y() += 0.01f / transformation.z();
		if (Input::GetKey(GLFW_KEY_A).down) transformation.x() -= 0.01f / transformation.z();
		if (Input::GetKey(GLFW_KEY_D).down) transformation.x() += 0.01f / transformation.z();
		if (Input::GetKey(GLFW_KEY_UP).down) transformation.z() += 0.01f * transformation.z();
		if (Input::GetKey(GLFW_KEY_DOWN).down) transformation.z() -= 0.01f * transformation.z();

		buffer.Update(&transformation[0], sizeof(point3D));

		pipeline.Bind(commandBuffer);
		descriptor.Bind(commandBuffer, pipeline.GetLayout());
		mesh.Bind(commandBuffer);
		vkCmdDrawIndexed(commandBuffer, CUI(mesh.GetIndices().size()), 1, 0, 0, 0);
	}
	else
	{
		mesh.Destroy();
		buffer.Destroy();
		descriptor.Destroy();
		pass.Destroy();
		pipeline.Destroy();

		start = true;
	}
}

void Manager::Create()
{
	try
	{
		CreateGLFW();
		CreateVulkan();
	}
	catch(const std::exception& e)
	{
		throw (std::runtime_error(e.what()));
	}
}

void Manager::CreateGLFW()
{
	if (!glfwInit()) throw (std::runtime_error("Failed to initiate GLFW"));

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	window.CreateFrame();
}

void Manager::CreateVulkan()
{
	Graphics::CreateInstance();
	device.CreatePhysical();
	window.CreateSurface(device);
	std::cout << "Window created: " << window << std::endl;
	device.SelectQueues();
	device.CreateLogical();
	device.RetrieveQueues();
	std::cout << "Device created: " << device << std::endl;
	swapchain.Create(&window, &device);
	std::cout << "Swapchain created: " << swapchain << std::endl;
	Renderer::Create(swapchain.GetFrameCount(), &device, &swapchain);
}

void Manager::Destroy()
{
	if (device.IsCreated() && vkDeviceWaitIdle(device.GetLogicalDevice()) != VK_SUCCESS)
		throw (std::runtime_error("Failed to wait for device to be idle"));

	DestroyGLFW();
	DestroyVulkan();
}

void Manager::DestroyGLFW()
{
	window.DestroyFrame();
	glfwTerminate();
}

void Manager::DestroyVulkan()
{
	Test(nullptr, 0);
	swapchain.Destroy();
	Renderer::Destroy();
	window.DestroySurface();
	device.Destroy();
	Graphics::DestroyInstance();
}

Window& Manager::GetWindow()
{
	return (window);
}

Device& Manager::GetDevice()
{
	return (device);
}

Swapchain& Manager::GetSwapchain()
{
	return (swapchain);
}

void Manager::Start()
{
	Test(nullptr, 0);

	Renderer::RegisterCall(Test);
}

void Manager::Frame()
{
	glfwPollEvents();

	Input::Frame();

	Renderer::Frame();
}

bool Manager::ShouldClose()
{
	return (glfwWindowShouldClose(window.GetData()));
}

Window Manager::window;
Device Manager::device;
Swapchain Manager::swapchain;