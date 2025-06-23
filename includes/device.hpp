#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct DeviceConfig
{
	bool discrete = false;
	bool tesselation = false;
	bool anisotropic = false;
};

struct QueueFamilies
{
	int graphicsFamily = -1;
	int computeFamily = -1;
	int presentFamily = -1;

	VkQueue graphicsQueue = nullptr;
	VkQueue computeQueue = nullptr;
	VkQueue presentQueue = nullptr;
};

class Device
{
	private:
		VkPhysicalDevice physicalDevice = nullptr;
		VkDevice logicalDevice = nullptr;
		QueueFamilies queueFamilies;

		void CreatePhysical(const DeviceConfig& config);
		void CreateLogical(const DeviceConfig& config);
		void CreateQueues();

	public:
		Device();
		~Device();

		void Create();
		void Create(const DeviceConfig& config);

		void Destroy();
};