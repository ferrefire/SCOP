#include "manager.hpp"

#include "graphics.hpp"
#include "descriptor.hpp"

#include "pipeline.hpp"
#include "mesh.hpp"
#include "buffer.hpp"
#include "pass.hpp"

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
}

void Manager::Destroy()
{
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
	swapchain.Destroy();
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
	//Buffer buffer;
	//BufferConfig bufferConfig{};
	//bufferConfig.size = 16;
	//buffer.Create(bufferConfig, &device);

	//Pass pass;
	//PassConfig passConfig = Pass::DefaultConfig();
	//pass.Create(passConfig, &device);

	//Pipeline pipeline;
	//PipelineConfig config = Pipeline::DefaultConfig();
	//config.shader = "screen";
	//config.vertexInfo = Mesh::GetVertexInfo(true, false, false, false);
	//config.renderpass = pass.GetRenderpass();
	//pipeline.Create(config, &device);
}

void Manager::Frame()
{
	glfwPollEvents();
}

bool Manager::ShouldClose()
{
	return (glfwWindowShouldClose(window.GetData()));
}

Window Manager::window;
Device Manager::device;
Swapchain Manager::swapchain;