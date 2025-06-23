#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Window
{
	private:
		uint32_t width = 400;
		uint32_t height = 400;
		GLFWwindow* data = nullptr;

	public:
		Window();
		~Window();

		void Create();

		void Destroy();
};