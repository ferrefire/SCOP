#include "mesh.hpp"

#include "printer.hpp"

#include <stdexcept>

MESH_TEMPLATE
Mesh<V, I>::Mesh()
{

}

MESH_TEMPLATE
Mesh<V, I>::~Mesh()
{

}

MESH_TEMPLATE
void Mesh<V, I>::Create()
{

}

MESH_TEMPLATE
void Mesh<V, I>::Destroy()
{
	
}

MESH_TEMPLATE
void Mesh<V, I>::AddVertex(Vertex<V> vertex)
{
	vertices.push_back(vertex);
}

MESH_TEMPLATE
Vertex<V> Mesh<V, I>::GetVertex(indexType index)
{
	if (index < 0 || index >= vertices.size()) throw (std::runtime_error("Index out of range"));

	return (vertices[index]);
}

MESH_TEMPLATE
Vertex<V> Mesh<V, I>::GetVertex(indexType index) const
{
	if (index < 0 || index >= vertices.size()) throw (std::runtime_error("Index out of range"));

	return (vertices[index]);
}

MESH_TEMPLATE
VertexInfo Mesh<V, I>::GetVertexInfo(VertexConfig config)
{
	VertexInfo vertexInfo{};

	vertexInfo.bindingCount = 1;
	vertexInfo.attributeCount = Bitmask::HasFlag(config, Position) + Bitmask::HasFlag(config, Coordinate) + 
		Bitmask::HasFlag(config, Normal) + Bitmask::HasFlag(config, Color);
	vertexInfo.attributeDescriptions.resize(vertexInfo.attributeCount);
	
	vertexInfo.bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	vertexInfo.bindingDescription.binding = 0;
	vertexInfo.bindingDescription.stride = 0;

	int index = 0;
	if (Bitmask::HasFlag(config, Position))
	{
		vertexInfo.attributeDescriptions[index].binding = 0;
		vertexInfo.attributeDescriptions[index].location = index;
		vertexInfo.attributeDescriptions[index].format = VK_FORMAT_R32G32B32_SFLOAT;
		vertexInfo.attributeDescriptions[index].offset = vertexInfo.bindingDescription.stride;
		vertexInfo.bindingDescription.stride += sizeof(point3D);
		index++;
	}
	if (Bitmask::HasFlag(config, Coordinate))
	{
		vertexInfo.attributeDescriptions[index].binding = 0;
		vertexInfo.attributeDescriptions[index].location = index;
		vertexInfo.attributeDescriptions[index].format = VK_FORMAT_R32G32_SFLOAT;
		vertexInfo.attributeDescriptions[index].offset = vertexInfo.bindingDescription.stride;
		vertexInfo.bindingDescription.stride += sizeof(point2D);
		index++;
	}
	if (Bitmask::HasFlag(config, Normal))
	{
		vertexInfo.attributeDescriptions[index].binding = 0;
		vertexInfo.attributeDescriptions[index].location = index;
		vertexInfo.attributeDescriptions[index].format = VK_FORMAT_R32G32B32_SFLOAT;
		vertexInfo.attributeDescriptions[index].offset = vertexInfo.bindingDescription.stride;
		vertexInfo.bindingDescription.stride += sizeof(point3D);
		index++;
	}
	if (Bitmask::HasFlag(config, Color))
	{
		vertexInfo.attributeDescriptions[index].binding = 0;
		vertexInfo.attributeDescriptions[index].location = index;
		vertexInfo.attributeDescriptions[index].format = VK_FORMAT_R32G32B32_SFLOAT;
		vertexInfo.attributeDescriptions[index].offset = vertexInfo.bindingDescription.stride;
		vertexInfo.bindingDescription.stride += sizeof(point3D);
		index++;
	}

	vertexInfo.floatCount = vertexInfo.bindingDescription.stride / 8;

	return (vertexInfo);
}

VERTEX_TEMPLATE
std::ostream& operator<<(std::ostream& out, Vertex<V> vertex)
{
	if constexpr (Bitmask::HasFlag(V, Position)) out << "position: " << vertex.position << " ";
	if constexpr (Bitmask::HasFlag(V, Coordinate)) out << "coordinate: " << vertex.coordinate << " ";
	if constexpr (Bitmask::HasFlag(V, Normal)) out << "normal: " << vertex.normal << " ";
	if constexpr (Bitmask::HasFlag(V, Color)) out << "color: " << vertex.color << " ";

	return (out);
}

MESH_TEMPLATE
std::ostream& operator<<(std::ostream& out, const Mesh<V, I>& mesh)
{
	out << mesh.GetVertex(0) << " ";

	return (out);
}