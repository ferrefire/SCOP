#include "manager.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <iostream>

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

void Manager::Destroy()
{
	window.Destroy();
	DestroyGLFW();
}

void Manager::DestroyGLFW()
{
	glfwTerminate();
}

Window Manager::window;