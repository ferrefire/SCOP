#pragma once

#include "device.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct WindowConfig
{
	VkSurfaceCapabilitiesKHR capabilities{};
	VkSurfaceFormatKHR format{};
	VkPresentModeKHR presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
	VkExtent2D extent{};
};

class Window
{
	private:
		uint32_t width = 400;
		uint32_t height = 400;
		GLFWwindow* data = nullptr;
		VkSurfaceKHR surface = nullptr;
		WindowConfig config{};

		bool CanPresent(Device& device);
		void SelectFormat(Device& device);
		void SelectPresentMode(Device& device);

	public:
		Window();
		~Window();

		void Create();
		void CreateSurface(Device& device);

		void Destroy();
		void DestroySurface();

		VkSurfaceKHR& GetSurface();
		WindowConfig GetConfig();
};