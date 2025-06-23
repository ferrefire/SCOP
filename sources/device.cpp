#include "device.hpp"

#include "graphics.hpp"

#include <stdexcept>
#include <vector>
#include <set>

Device::Device()
{

}

Device::~Device()
{

}

void Device::Create()
{
	DeviceConfig config;
	Create(config);
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
		VkDeviceQueueCreateInfo queueCreateInfo;
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures;
	if (config.tesselation) deviceFeatures.tessellationShader = VK_TRUE;
	if (config.anisotropic) deviceFeatures.samplerAnisotropy = VK_TRUE;

	std::vector<const char*> deviceExtensions;

	VkDeviceCreateInfo deviceCreateInfo;
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
	deviceCreateInfo.enabledExtensionCount = 0;
	deviceCreateInfo.ppEnabledExtensionNames = nullptr;

	if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice) != VK_SUCCESS)
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

}