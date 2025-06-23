#include "graphics.hpp"

#include <stdexcept>
#include <string>
#include <iostream>

Graphics::Graphics()
{

}

Graphics::~Graphics()
{
	Destroy();
}

void Graphics::Create()
{
	if (instance) throw (std::runtime_error("Vulkan instance already exists"));

	if (HasValidationLayers()) validationLayersEnabled = true;
	std::cout << "Vulkan validation layers: " << (validationLayersEnabled ? "enabled" : "disabled") << std::endl;

	VkApplicationInfo appInfo;
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "scop";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "None";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_3;

	VkInstanceCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.ppEnabledExtensionNames = glfwGetRequiredInstanceExtensions(&createInfo.enabledExtensionCount);
	createInfo.enabledLayerCount = validationLayers.size();
	createInfo.ppEnabledLayerNames = validationLayers.data();

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
	{
		throw (std::runtime_error("Failed to create vulkan instance"));
	}
}

void Graphics::Destroy()
{
	if (instance)
	{
		vkDestroyInstance(instance, nullptr);
		instance = nullptr;
	}
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

VkInstance& Graphics::GetInstance()
{
	if (!instance) throw (std::runtime_error("Instance requested but not yet created"));

	return (instance);
}

VkInstance Graphics::instance = nullptr;
bool Graphics::validationLayersEnabled = false;
std::vector<const char*> Graphics::validationLayers = {"VK_LAYER_KHRONOS_validation"};