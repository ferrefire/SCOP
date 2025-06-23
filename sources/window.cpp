#include "window.hpp"

#include <stdexcept>
#include <iostream>

Window::Window()
{

}

Window::~Window()
{
	Destroy();
}

void Window::Create()
{
	if (data) throw (std::runtime_error("Window already exists"));

	data = glfwCreateWindow(width, height, "scop", nullptr, nullptr);

	if (!data) throw (std::runtime_error("Failed to create window"));

	std::cout << "Window succesfully created" << std::endl;
}

void Window::Destroy()
{
	if (data)
	{
		glfwDestroyWindow(data);
		data = nullptr;
	}
}