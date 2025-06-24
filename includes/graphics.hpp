#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class Graphics
{
	private:
		static VkInstance instance;
		static VkSwapchainKHR swapchain;
		static std::vector<VkImage> swapchainImages;

		static bool validationLayersEnabled;
		static std::vector<const char*> validationLayers;

		static bool HasValidationLayers();

	public:
		Graphics();
		~Graphics();

		static void CreateInstance();
		static void CreateSwapchain();

		static void DestroyInstance();
		static void DestroySwapchain();

		static VkInstance& GetInstance();
};