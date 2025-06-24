#include "window.hpp"

#include "graphics.hpp"
#include "utilities.hpp"

#include <stdexcept>
#include <iostream>

Window::Window()
{

}

Window::~Window()
{
	DestroyFrame();
	DestroySurface();
}

void Window::CreateFrame()
{
	if (data) throw (std::runtime_error("Window already exists"));

	data = glfwCreateWindow(width, height, "scop", nullptr, nullptr);

	if (!data) throw (std::runtime_error("Failed to create window"));

	std::cout << "Window succesfully created" << std::endl;
}

void Window::CreateSurface(Device& device)
{
	if (!data) throw (std::runtime_error("Window does not exist yet"));

	if (glfwCreateWindowSurface(Graphics::GetInstance(), data, nullptr, &surface) != VK_SUCCESS)
		throw (std::runtime_error("Failed to create window surface"));

	//if (!CanPresent(device)) throw (std::runtime_error("Window surface can not be presented to"));

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device.GetPhysicalDevice(), surface, &config.capabilities);
	SelectFormat(device);
	SelectPresentMode(device);

	config.extent.width = width;
	config.extent.height = height;
}

void Window::DestroyFrame()
{
	if (data)
	{
		glfwDestroyWindow(data);
		data = nullptr;
	}
}

void Window::DestroySurface()
{
	if (surface)
	{
		vkDestroySurfaceKHR(Graphics::GetInstance(), surface, nullptr);
		surface = nullptr;
	}

	config = WindowConfig{};
}

VkSurfaceKHR& Window::GetSurface()
{
	if (!surface) throw (std::runtime_error("Window surface requested but not yet created"));

	return (surface);
}

WindowConfig Window::GetConfig()
{
	return (config);
}

bool Window::CanPresent(Device& device, int presentQueueIndex = -1)
{
	if (!surface) throw (std::runtime_error("Window surface does not exist yet"));

	uint32_t queueIndex = (presentQueueIndex == -1 ? device.GetQueueIndex(QueueType::Present) : CUI(presentQueueIndex));
	VkBool32 canPresent = false;
	vkGetPhysicalDeviceSurfaceSupportKHR(device.GetPhysicalDevice(), queueIndex, surface, &canPresent);

	return (canPresent);
}

void Window::SelectFormat(Device& device)
{
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device.GetPhysicalDevice(), surface, &formatCount, nullptr);
	
	std::vector<VkSurfaceFormatKHR> formats(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(device.GetPhysicalDevice(), surface, &formatCount, formats.data());

	bool correctFormatFound = false;
	for (VkSurfaceFormatKHR availableFormat : formats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			config.format = availableFormat;
			correctFormatFound = true;
		}
	}

	if (!correctFormatFound) throw (std::runtime_error("Failed to find correct surface format"));
}

void Window::SelectPresentMode(Device& device)
{
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device.GetPhysicalDevice(), surface, &presentModeCount, nullptr);

	std::vector<VkPresentModeKHR> presentModes(presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(device.GetPhysicalDevice(), surface, &presentModeCount, presentModes.data());

	bool correctPresentModeFound = false;
	for (VkPresentModeKHR availablePresentMode : presentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_FIFO_KHR)
		{
			config.presentMode = availablePresentMode;
			correctPresentModeFound = true;
		}
	}

	if (!correctPresentModeFound) throw (std::runtime_error("Failed to find correct surface present mode"));
}