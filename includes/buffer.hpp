#pragma once

#include "device.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

struct BufferConfig
{
	bool mapped = false;
	VkDeviceSize size = 0;
	VkBufferUsageFlags usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE;
};

class Buffer
{
	private:
		BufferConfig config{};
		Device* device = nullptr;

		VkBuffer buffer = nullptr;
		VkDeviceMemory memory = nullptr;
		void* address = nullptr;

		void CreateBuffer();
		void AllocateMemory();

	public:
		Buffer();
		~Buffer();

		void Create(BufferConfig bufferConfig, Device* bufferDevice);

		void Destroy();

		BufferConfig GetConfig();
};

std::ostream& operator<<(std::ostream& out, Buffer& buffer);