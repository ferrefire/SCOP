#include "manager.hpp"

#include "graphics.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <iostream>

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
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error during manager creation: " << e.what() << std::endl;
		throw (std::runtime_error("Failed to create manager"));
	}
}

void Manager::CreateGLFW()
{
	if (!glfwInit()) throw (std::runtime_error("Failed to initiate GLFW"));

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	window.Create();
}

void Manager::CreateVulkan()
{
	Graphics::Create();
	device.Create();
}

void Manager::Destroy()
{
	DestroyGLFW();
	DestroyVulkan();
}

void Manager::DestroyGLFW()
{
	window.Destroy();
	glfwTerminate();
}

void Manager::DestroyVulkan()
{
	device.Destroy();
	Graphics::Destroy();
}

Window Manager::window;