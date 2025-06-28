#include "pipeline.hpp"

#include "utilities.hpp"

#include <stdexcept>
#include <string>

Pipeline::Pipeline()
{

}

Pipeline::~Pipeline()
{

}

void Pipeline::Create(const PipelineConfig& pipelineConfig, Device* pipelineDevice)
{
	config = pipelineConfig;
	device = pipelineDevice;

	std::string path = Utilities::GetPath();

	VkShaderModule vertexShader = CreateShader(Utilities::FileToBinary(path + "/shaders/" + config.shader + ".vert.spv"));
	VkShaderModule fragmentShader = CreateShader(Utilities::FileToBinary(path + "/shaders/" + config.shader + ".frag.spv"));

	//Continue from here
}

VkShaderModule Pipeline::CreateShader(const std::vector<char>& code)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

	VkShaderModule shaderModule = nullptr;
	if (vkCreateShaderModule(device->GetLogicalDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
		throw std::runtime_error("Failed to create shader");

	return (shaderModule);
}

void Pipeline::CreateLayout()
{
	if (layout) throw (std::runtime_error("Pipeline layout already exists"));
	if (!device) throw (std::runtime_error("Pipeline has no device"));


}