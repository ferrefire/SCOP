#include "device.hpp"

#include "graphics.hpp"

#include <stdexcept>
#include <vector>
#include <set>
#include <iostream>
#include <string>

Device::Device()
{

}

Device::~Device()
{
	Destroy();
}

void Device::Create()
{
	DeviceConfig config{};
	Create(config);

	PrintProperties();
}

void Device::Create(const DeviceConfig& config)
{
	CreatePhysical(config);
	CreateLogical(config);
	CreateQueues();
}

void Device::CreatePhysical(const DeviceConfig& config)
{
	if (physicalDevice) throw (std::runtime_error("Physical device already exists"));

	uint32_t deviceCount;
	vkEnumeratePhysicalDevices(Graphics::GetInstance(), &deviceCount, nullptr);
	if (deviceCount <= 0) throw (std::runtime_error("Failed to find a GPU with Vulkan support"));

	std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
	vkEnumeratePhysicalDevices(Graphics::GetInstance(), &deviceCount, physicalDevices.data());

	for (const VkPhysicalDevice& device : physicalDevices)
	{
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		if (config.discrete && deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) continue;
		else if (config.tesselation && !deviceFeatures.tessellationShader) continue;
		else if (config.anisotropic && !deviceFeatures.samplerAnisotropy) continue;

		uint32_t queueCount;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueCount);
		for (int i = 0; i < queueCount; i++)
		{
			if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				queueFamilies.graphicsFamily = i;
				queueFamilies.presentFamily = i;
				break;
			}
		}

		if (queueFamilies.graphicsFamily == -1) continue;

		physicalDevice = device;
	}

	if (!physicalDevice) throw (std::runtime_error("Failed to find a suitable GPU"));
}

void Device::CreateLogical(const DeviceConfig& config)
{
	if (logicalDevice) throw (std::runtime_error("Logical device already exists"));
	if (!physicalDevice) throw (std::runtime_error("Physical device does not exist"));

	float queuePriority = 1.0f;
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = {static_cast<uint32_t>(queueFamilies.graphicsFamily), static_cast<uint32_t>(queueFamilies.presentFamily)};

	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		createInfo.queueFamilyIndex = queueFamily;
		createInfo.queueCount = 1;
		createInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(createInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{};
	if (config.tesselation) deviceFeatures.tessellationShader = VK_TRUE;
	if (config.anisotropic) deviceFeatures.samplerAnisotropy = VK_TRUE;

	std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = queueCreateInfos.size();
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = deviceExtensions.size();
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS)
		throw (std::runtime_error("Failed to create logical device"));
}

void Device::CreateQueues()
{
	if (queueFamilies.graphicsFamily || queueFamilies.presentFamily) throw (std::runtime_error("Queues already exist"));
	if (!logicalDevice) throw (std::runtime_error("Logical device does not exist"));

	vkGetDeviceQueue(logicalDevice, queueFamilies.graphicsFamily, 0, &queueFamilies.graphicsQueue);
	vkGetDeviceQueue(logicalDevice, queueFamilies.presentFamily, 0, &queueFamilies.presentQueue);
}

void Device::Destroy()
{
	if (logicalDevice)
	{
		vkDestroyDevice(logicalDevice, nullptr);
		logicalDevice = nullptr;
		physicalDevice = nullptr;
		queueFamilies = QueueFamilies{};
	}
}

VkPhysicalDevice& Device::GetPhysicalDevice()
{
	if (!physicalDevice) throw (std::runtime_error("Physical device requested but not yet created"));

	return (physicalDevice);
}

uint32_t Device::GetQueueIndex(QueueType type)
{
	int index = -1;

	switch (type)
	{
		case QueueType::Graphics: index = queueFamilies.graphicsFamily;
		case QueueType::Compute: index = queueFamilies.computeFamily;
		case QueueType::Present: index = queueFamilies.presentFamily;
		default: index = -1;
	}

	if (index == -1) throw (std::runtime_error("Queue index does not exist"));

	return (static_cast<uint32_t>(index));
}

void Device::PrintProperties()
{
	if (!physicalDevice || !logicalDevice)
	{
		std::cout << "Device is not complete" << std::endl;
		return;
	}

	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

	std::string type;
	switch (deviceProperties.deviceType)
	{
		case 0: type = "Other";
		case 1: type = "Integrated";
		case 2: type = "Discrete";
		case 3: type = "Virtual";
		case 4: type = "CPU";
		default: type = "Unknown";
	}

	std::cout << "-Device properties-" << std::endl;
	std::cout << "Name: " << deviceProperties.deviceName << std::endl;
	std::cout << "Vendor: " << deviceProperties.vendorID << std::endl;
	std::cout << "Type: " << type << std::endl;
	std::cout << "Driver version: " << deviceProperties.driverVersion << std::endl;
	std::cout << "API version: " << deviceProperties.apiVersion << std::endl;
}