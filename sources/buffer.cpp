#include "buffer.hpp"

#include "printer.hpp"

#include <stdexcept>

Buffer::Buffer()
{

}

Buffer::~Buffer()
{
	Destroy();
}

void Buffer::Create(const BufferConfig& bufferConfig, Device* bufferDevice)
{
	config = bufferConfig;
	device = bufferDevice;

	CreateBuffer();
	AllocateMemory();
}

void Buffer::CreateBuffer()
{
	if (buffer) throw (std::runtime_error("Buffer already exists"));
	if (!device) throw (std::runtime_error("Buffer has no device"));

	VkBufferCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	createInfo.size = config.size;
	createInfo.usage = config.usage;
	createInfo.sharingMode = config.sharingMode;

	if (vkCreateBuffer(device->GetLogicalDevice(), &createInfo, nullptr, &buffer) != VK_SUCCESS)
		throw (std::runtime_error("Failed to create buffer"));
}

void Buffer::AllocateMemory()
{
	if (memory) throw (std::runtime_error("Memory is already allocated"));
	if (!device) throw (std::runtime_error("Buffer has no device"));

	VkDevice& logicalDevice = device->GetLogicalDevice();

	VkMemoryRequirements requirements{};
	vkGetBufferMemoryRequirements(logicalDevice, buffer, &requirements);

	VkMemoryAllocateInfo allocateInfo{};
	allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocateInfo.allocationSize = requirements.size;
	allocateInfo.memoryTypeIndex = device->FindMemoryType(requirements.memoryTypeBits, config.properties);

	if (vkAllocateMemory(logicalDevice, &allocateInfo, nullptr, &memory) != VK_SUCCESS)
		throw (std::runtime_error("Failed to allocate memory"));

	if (vkBindBufferMemory(logicalDevice, buffer, memory, 0) != VK_SUCCESS)
		throw (std::runtime_error("Failed to bind memory"));

	if (config.mapped)
	{
		if (vkMapMemory(logicalDevice, memory, 0, config.size, 0, &address) != VK_SUCCESS)
			throw (std::runtime_error("Failed to map memory"));
	}
}

void Buffer::Destroy()
{
	if (!device) return;

	if (buffer)
	{
		vkDestroyBuffer(device->GetLogicalDevice(), buffer, nullptr);
		buffer = nullptr;
	}

	if (memory)
	{
		if (address)
		{
			vkUnmapMemory(device->GetLogicalDevice(), memory);
			address = nullptr;
		}

		vkFreeMemory(device->GetLogicalDevice(), memory, nullptr);
		memory = nullptr;
	}
}

BufferConfig Buffer::GetConfig()
{
	return (config);
}

std::ostream& operator<<(std::ostream& out, Buffer& buffer)
{
	BufferConfig config = buffer.GetConfig();

	out << std::endl;
	out << FLAG_VAL(config.usage, VkBufferUsageFlagBits) << std::endl;
	out << VAR_VAL(config.size) << std::endl;
	out << FLAG_VAL(config.properties, VkMemoryPropertyFlagBits) << std::endl;
	out << ENUM_VAL(config.sharingMode) << std::endl;
	out << VAR_VAL(config.mapped) << std::endl;

	return (out);
}