#include "graphics.hpp"

#include "manager.hpp"

#include <stdexcept>
#include <string>
#include <iostream>

Graphics::Graphics()
{

}

Graphics::~Graphics()
{
	DestroyInstance();
}

void Graphics::CreateInstance()
{
	if (instance) throw (std::runtime_error("Vulkan instance already exists"));

	if (HasValidationLayers()) validationLayersEnabled = true;
	std::cout << "Vulkan validation layers: " << (validationLayersEnabled ? "enabled" : "disabled") << std::endl << std::endl;

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "scop";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "None";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_3;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.ppEnabledExtensionNames = glfwGetRequiredInstanceExtensions(&createInfo.enabledExtensionCount);
	createInfo.enabledLayerCount = validationLayers.size();
	createInfo.ppEnabledLayerNames = validationLayers.data();

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
	{
		throw (std::runtime_error("Failed to create Vulkan instance"));
	}

	std::cout << "Vulkan instance created" << std::endl << std::endl;
}

void Graphics::CreateSwapchain()
{
	if (swapchain) throw (std::runtime_error("Vulkan swapchain already exists"));

	Window& window = Manager::GetWindow();
	Device& device = Manager::GetDevice();

	WindowConfig windowConfig = window.GetConfig();

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = window.GetSurface();
	createInfo.minImageCount = windowConfig.capabilities.minImageCount;
	createInfo.imageFormat = windowConfig.format.format;
	createInfo.imageColorSpace = windowConfig.format.colorSpace;
	createInfo.imageExtent = windowConfig.extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	createInfo.preTransform = windowConfig.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = windowConfig.presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = nullptr;

	if (vkCreateSwapchainKHR(device.GetLogicalDevice(), &createInfo, nullptr, &swapchain) != VK_SUCCESS)
		throw (std::runtime_error("Failed to create Vulkan swapchain"));

	uint32_t imageCount = 0;
	vkGetSwapchainImagesKHR(device.GetLogicalDevice(), swapchain, &imageCount, nullptr);
	swapchainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device.GetLogicalDevice(), swapchain, &imageCount, swapchainImages.data());

	std::cout << "Vulkan swapchain created" << std::endl << std::endl;
}

void Graphics::DestroyInstance()
{
	if (instance)
	{
		vkDestroyInstance(instance, nullptr);
		instance = nullptr;
	}
}

void Graphics::DestroySwapchain()
{
	if (swapchain)
	{
		vkDestroySwapchainKHR(Manager::GetDevice().GetLogicalDevice(), swapchain, nullptr);
		swapchain = nullptr;
		swapchainImages.clear();
	}
}

VkInstance& Graphics::GetInstance()
{
	if (!instance) throw (std::runtime_error("Instance requested but not yet created"));

	return (instance);
}

bool Graphics::HasValidationLayers()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layer : validationLayers)
	{
		bool layerFound = false;

		for (const VkLayerProperties &layerProperties : availableLayers)
		{
			if (std::string(layerProperties.layerName).compare(layer) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound) return (false);
	}

	return (true);
}

VkInstance Graphics::instance = nullptr;
VkSwapchainKHR Graphics::swapchain = nullptr;
std::vector<VkImage> Graphics::swapchainImages;

bool Graphics::validationLayersEnabled = false;
std::vector<const char*> Graphics::validationLayers = {"VK_LAYER_KHRONOS_validation"};