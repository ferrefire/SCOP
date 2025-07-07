#pragma once

#include "buffer.hpp"
#include "point.hpp"
#include "device.hpp"
#include "bitmask.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <variant>
#include <type_traits>
#include <iostream>
#include <string>

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

#define MESH_TEMPLATE template <VertexConfig V, VkIndexType I>
#define VERTEX_TEMPLATE template <VertexConfig V>

struct PositionStruct { point3D position; };
struct CoordinateStruct { point2D coordinate; };
struct NormalStruct { point3D normal; };
struct ColorStruct { point3D color; };

VERTEX_TEMPLATE
struct Empty {};

VERTEX_TEMPLATE
struct Vertex :
	std::conditional_t<Bitmask::HasFlag(V, Position), PositionStruct, Empty<Position>>,
	std::conditional_t<Bitmask::HasFlag(V, Coordinate), CoordinateStruct, Empty<Coordinate>>,
	std::conditional_t<Bitmask::HasFlag(V, Normal), NormalStruct, Empty<Normal>>,
	std::conditional_t<Bitmask::HasFlag(V, Color), ColorStruct, Empty<Color>>
{};

MESH_TEMPLATE
class Mesh
{
	using indexType = typename std::conditional_t<I == VK_INDEX_TYPE_UINT16, uint16_t, uint32_t>;

	private:
		MeshConfig config{};
		Device* device = nullptr;

		std::vector<float> data;
		std::vector<Vertex<V>> vertices;
		std::vector<indexType> indices;

		Buffer vertexBuffer;
		Buffer indexBuffer;

	public:
		Mesh();
		~Mesh();

		void Create();

		void Destroy();

		void AddVertex(Vertex<V> vertex);
		Vertex<V> GetVertex(indexType index);
		Vertex<V> GetVertex(indexType index) const;

		static VertexInfo GetVertexInfo(VertexConfig config);
};

VERTEX_TEMPLATE
std::ostream& operator<<(std::ostream& out, Vertex<V> vertex);

MESH_TEMPLATE
std::ostream& operator<<(std::ostream& out, const Mesh<V, I>& mesh);

#include "mesh.tpp"