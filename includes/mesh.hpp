#pragma once

#include "buffer.hpp"
#include "point.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

enum class VertexConfig 
{
	None = 0,
	Position = 1 << 0,
	Coordinate = 1 << 1,
	Normal = 1 << 2,
	Color = 1 << 3,
};

struct MeshConfig
{
	bool positions = true;
	bool coordinates = false;
	bool normals = false;
	bool colors = false;
};

struct VertexInfo
{
	uint32_t bindingCount = 0;
	uint32_t attributeCount = 0;
	uint32_t floatCount = 0;
	VkVertexInputBindingDescription bindingDescription{};
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
};

class Mesh
{
	private:
		MeshConfig config{};

		Buffer vertexBuffer;
		Buffer indexBuffer;

	public:
		Mesh();
		~Mesh();

		void Create(const MeshConfig& meshConfig);

		void Destroy();

		static VertexInfo GetVertexInfo(bool position, bool coordinate, bool normal, bool color);
};