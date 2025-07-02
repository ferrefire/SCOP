#pragma once

#include "buffer.hpp"
#include "point.hpp"
#include "device.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <variant>

typedef enum VertexConfigBits
{
	None = 0,
	Position = 1 << 0,
	Coordinate = 1 << 1,
	Normal = 1 << 2,
	Color = 1 << 3,
} VertexConfigBits;
typedef uint32_t VertexConfig;

struct VertexInfo
{
	uint32_t bindingCount = 0;
	uint32_t attributeCount = 0;
	uint32_t floatCount = 0;
	VkVertexInputBindingDescription bindingDescription{};
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
};

struct MeshConfig
{
	VertexConfig vertexConfig = Position;
	VkIndexType indexType = VK_INDEX_TYPE_UINT16;
};

//#define MESH_TEMPLATE template <VertexConfig V, VkIndexType I>

//MESH_TEMPLATE
class Mesh
{
	private:
		MeshConfig config{};
		Device* device = nullptr;

		std::vector<float> vertices;
		//std::vector<uint16_t> indices;
		//std::variant<std::vector<uint16_t>, std::vector<uint32_t>> indices;

		Buffer vertexBuffer;
		Buffer indexBuffer;

	public:
		Mesh();
		~Mesh();

		void Create();

		void Destroy();

		static VertexInfo GetVertexInfo(bool position, bool coordinate, bool normal, bool color);
};