#include "manager.hpp"

#include "graphics.hpp"
#include "buffer.hpp"

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

		Buffer testBuffer;
		testBuffer.Create(BufferConfig{false, sizeof(uint32_t)}, &device);
		std::cout << "Buffer created: " << testBuffer << std::endl;
		testBuffer.Destroy();
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
	Graphics::CreateSwapchain();
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
	Graphics::DestroySwapchain();
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