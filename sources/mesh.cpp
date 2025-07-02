#include "mesh.hpp"

Mesh::Mesh()
{
	//indices = std::vector<uint16_t>(10);
	//std::get<std::vector<uint16_t>>(indices)
}

Mesh::~Mesh()
{

}

VertexInfo Mesh::GetVertexInfo(bool position, bool coordinate, bool normal, bool color)
{
	VertexInfo vertexInfo{};

	vertexInfo.bindingCount = 1;
	vertexInfo.attributeCount = position + coordinate + normal + color;
	vertexInfo.attributeDescriptions.resize(vertexInfo.attributeCount);
	
	vertexInfo.bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	vertexInfo.bindingDescription.binding = 0;
	vertexInfo.bindingDescription.stride = 0;

	int index = 0;
	if (position)
	{
		vertexInfo.attributeDescriptions[index].binding = 0;
		vertexInfo.attributeDescriptions[index].location = index;
		vertexInfo.attributeDescriptions[index].format = VK_FORMAT_R32G32B32_SFLOAT;
		vertexInfo.attributeDescriptions[index].offset = vertexInfo.bindingDescription.stride;
		vertexInfo.bindingDescription.stride += sizeof(point3D);
		index++;
	}
	if (coordinate)
	{
		vertexInfo.attributeDescriptions[index].binding = 0;
		vertexInfo.attributeDescriptions[index].location = index;
		vertexInfo.attributeDescriptions[index].format = VK_FORMAT_R32G32_SFLOAT;
		vertexInfo.attributeDescriptions[index].offset = vertexInfo.bindingDescription.stride;
		vertexInfo.bindingDescription.stride += sizeof(point2D);
		index++;
	}
	if (normal)
	{
		vertexInfo.attributeDescriptions[index].binding = 0;
		vertexInfo.attributeDescriptions[index].location = index;
		vertexInfo.attributeDescriptions[index].format = VK_FORMAT_R32G32B32_SFLOAT;
		vertexInfo.attributeDescriptions[index].offset = vertexInfo.bindingDescription.stride;
		vertexInfo.bindingDescription.stride += sizeof(point3D);
		index++;
	}
	if (color)
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