#include "descriptor.hpp"

#include "manager.hpp"
#include "printer.hpp"

#include <stdexcept>

Descriptor::Descriptor()
{

}

Descriptor::~Descriptor()
{
	Destroy();
}

void Descriptor::Create(const std::vector<DescriptorConfig>& descriptorConfig, Device* descriptorDevice = nullptr)
{
	config = descriptorConfig;
	device = descriptorDevice;

	if (!device) device = &Manager::GetDevice();

	CreateLayout();
	CreatePool();
	AllocateSet();
}

void Descriptor::CreateLayout()
{
	if (layout) throw (std::runtime_error("Descriptor layout already exists"));
	if (!device) throw (std::runtime_error("Descriptor has no device"));

	std::vector<VkDescriptorSetLayoutBinding> layoutBindings(config.size());
	for (int i = 0; i < config.size(); i++)
	{
		layoutBindings[i].binding = i;
		layoutBindings[i].descriptorType = config[i].type;
		layoutBindings[i].descriptorCount = config[i].count;
		layoutBindings[i].stageFlags = config[i].stages;
	}

	VkDescriptorSetLayoutCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	createInfo.bindingCount = config.size();
	createInfo.pBindings = layoutBindings.data();

	if (vkCreateDescriptorSetLayout(device->GetLogicalDevice(), &createInfo, nullptr, &layout) != VK_SUCCESS)
		throw (std::runtime_error("Failed to create descriptor layout"));
}

void Descriptor::CreatePool()
{
	if (pool) throw (std::runtime_error("Descriptor pool already exists"));
	if (!device) throw (std::runtime_error("Descriptor has no device"));

	std::vector<VkDescriptorPoolSize> poolSizes(config.size());
	for (int i = 0; i < config.size(); i++)
	{
		poolSizes[i].type = config[i].type;
		poolSizes[i].descriptorCount = config[i].count;
	}

	VkDescriptorPoolCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	createInfo.poolSizeCount = poolSizes.size();
	createInfo.pPoolSizes = poolSizes.data();
	createInfo.maxSets = 1;

	if (vkCreateDescriptorPool(device->GetLogicalDevice(), &createInfo, nullptr, &pool) != VK_SUCCESS)
		throw (std::runtime_error("Failed to create descriptor pool"));
}

void Descriptor::AllocateSet()
{
	if (set) throw (std::runtime_error("Descriptor set is already allocated"));
	if (!layout) throw (std::runtime_error("Descriptor has no layout"));
	if (!pool) throw (std::runtime_error("Descriptor has no pool"));
	if (!device) throw (std::runtime_error("Descriptor has no device"));

	VkDescriptorSetAllocateInfo allocateInfo{};
	allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocateInfo.descriptorPool = pool;
	allocateInfo.descriptorSetCount = 1;
	allocateInfo.pSetLayouts = &layout;

	if (vkAllocateDescriptorSets(device->GetLogicalDevice(), &allocateInfo, &set) != VK_SUCCESS)
		throw (std::runtime_error("Failed to allocate descriptor set"));
}

void Descriptor::Destroy()
{
	if (!device) return;

	if (pool)
	{
		vkDestroyDescriptorPool(device->GetLogicalDevice(), pool, nullptr);
		pool = nullptr;

		set = nullptr;
	}

	if (layout)
	{
		vkDestroyDescriptorSetLayout(device->GetLogicalDevice(), layout, nullptr);
		layout = nullptr;
	}
}

std::vector<DescriptorConfig> Descriptor::GetConfig()
{
	return (config);
}

void Descriptor::Update(uint32_t binding, VkDescriptorBufferInfo* bufferInfo = nullptr, VkDescriptorImageInfo* imageInfo = nullptr)
{
	if (!set) throw (std::runtime_error("Descriptor has no set"));
	if (!device) throw (std::runtime_error("Descriptor has no device"));

	VkWriteDescriptorSet writeInfo{};
	writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeInfo.dstSet = set;
	writeInfo.dstBinding = binding;
	writeInfo.dstArrayElement = 0;
	writeInfo.descriptorType = config[binding].type;
	writeInfo.descriptorCount = 1;
	writeInfo.pBufferInfo = bufferInfo;
	writeInfo.pImageInfo = imageInfo;

	vkUpdateDescriptorSets(device->GetLogicalDevice(), 1, &writeInfo, 0, nullptr);
}

std::ostream& operator<<(std::ostream& out, DescriptorConfig& config)
{
	out << std::endl;
	out << ENUM_VAL(config.type) << std::endl;
	out << FLAG_VAL(config.stages, VkShaderStageFlagBits) << std::endl;
	out << VAR_VAL(config.count) << std::endl;

	return (out);
}

std::ostream& operator<<(std::ostream& out, Descriptor& descriptor)
{
	for (DescriptorConfig& config : descriptor.GetConfig())
	{
		out << config;
	}

	return (out);
}