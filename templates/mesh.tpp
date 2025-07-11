#include "mesh.hpp"

#include "manager.hpp"
#include "printer.hpp"

#include <stdexcept>

MESH_TEMPLATE
Mesh<V, I>::Mesh()
{

}

MESH_TEMPLATE
Mesh<V, I>::~Mesh()
{
	Destroy();
}

MESH_TEMPLATE
void Mesh<V, I>::Create(Device* meshDevice)
{
	device = meshDevice;

	if (!device) device = &Manager::GetDevice();

	CreateData();
	CreateVertexBuffer();
	CreateIndexBuffer();
}

MESH_TEMPLATE
void Mesh<V, I>::CreateData()
{
	if (data.size() != 0) throw (std::runtime_error("Mesh data already exists"));
	if (vertices.size() <= 0) throw (std::runtime_error("Mesh has no vertices"));

	data.resize((sizeof(vertices[0]) / sizeof(float)) * vertices.size());
	uint32_t i = 0;
	for (Vertex<V> vertex : vertices)
	{
		for (float value : vertex.GetData())
		{
			data[i++] = value;
		}
	}
}

MESH_TEMPLATE
void Mesh<V, I>::CreateVertexBuffer()
{
	//if (vertexBuffer.GetBuffer()) throw (std::runtime_error("Mesh vertex buffer already exists"));
	if (data.size() == 0) throw (std::runtime_error("Mesh has no data"));
	if (!device) throw (std::runtime_error("Mesh has no device"));

	BufferConfig bufferConfig = Buffer::VertexConfig();
	bufferConfig.size = static_cast<VkDeviceSize>(sizeof(data[0]) * data.size());

	vertexBuffer.Create(bufferConfig, device, data.data());
}

MESH_TEMPLATE
void Mesh<V, I>::CreateIndexBuffer()
{
	if (noIndices) throw (std::runtime_error("Can't create index buffer because mesh is not indexed"));
	//if (indexBuffer.GetBuffer()) throw (std::runtime_error("Mesh index buffer already exists"));
	if (indices.size() == 0) throw (std::runtime_error("Mesh has no indices"));
	if (!device) throw (std::runtime_error("Mesh has no device"));

	BufferConfig bufferConfig = Buffer::IndexConfig();
	bufferConfig.size = static_cast<VkDeviceSize>(sizeof(indices[0]) * indices.size());

	indexBuffer.Create(bufferConfig, device, indices.data());
}

MESH_TEMPLATE
void Mesh<V, I>::Destroy()
{
	vertexBuffer.Destroy();
	indexBuffer.Destroy();

	data.clear();
	vertices.clear();
	indices.clear();
}

MESH_TEMPLATE
const std::vector<Vertex<V>>& Mesh<V, I>::GetVertices() const
{
	if (vertices.size() <= 0) throw (std::runtime_error("Mesh vertices requested but does not exist"));

	return (vertices);
}

MESH_TEMPLATE
const std::vector<std::conditional_t<I == VK_INDEX_TYPE_UINT16, uint16_t, uint32_t>>& Mesh<V, I>::GetIndices() const
{
	if (indices.size() <= 0) throw (std::runtime_error("Mesh indices requested but does not exist"));

	return (indices);
}

MESH_TEMPLATE
const std::vector<float>& Mesh<V, I>::GetData() const
{
	if (data.size() <= 0) throw (std::runtime_error("Mesh data requested but does not exist"));

	return (data);
}

MESH_TEMPLATE
void Mesh<V, I>::SetVertices(const std::vector<Vertex<V>>& newVertices)
{
	vertices.clear();
	vertices = newVertices;
}

MESH_TEMPLATE
void Mesh<V, I>::AddVertex(Vertex<V> vertex)
{
	vertices.push_back(vertex);
}

MESH_TEMPLATE
void Mesh<V, I>::SetIndices(const std::vector<indexType>& newIndices)
{
	indices.clear();
	indices = newIndices;
}

MESH_TEMPLATE
void Mesh<V, I>::AddIndex(indexType index)
{
	indices.push_back(index);
}

MESH_TEMPLATE
void Mesh<V, I>::Bind(VkCommandBuffer commandBuffer)
{
	if (!commandBuffer) throw (std::runtime_error("Can't bind mesh because the command buffer does not exist"));
	//if (!vertexBuffer.GetBuffer()) throw (std::runtime_error("Mesh vertex buffer does not exist"));
	//if (!noIndices && !indexBuffer.GetBuffer()) throw (std::runtime_error("Mesh index buffer does not exist"));

	VkDeviceSize offsets[]{0};

	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer.GetBuffer(), offsets);
	if (!noIndices) vkCmdBindIndexBuffer(commandBuffer, indexBuffer.GetBuffer(), 0, I);
}

MESH_TEMPLATE
VertexInfo Mesh<V, I>::GetVertexInfo()
{
	VertexInfo vertexInfo{};

	vertexInfo.bindingCount = 1;
	vertexInfo.attributeCount = Bitmask::HasFlag(V, Position) + Bitmask::HasFlag(V, Coordinate) + 
		Bitmask::HasFlag(V, Normal) + Bitmask::HasFlag(V, Color);
	vertexInfo.attributeDescriptions.resize(vertexInfo.attributeCount);
	
	vertexInfo.bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	vertexInfo.bindingDescription.binding = 0;
	vertexInfo.bindingDescription.stride = 0;

	int index = 0;
	if (Bitmask::HasFlag(V, Position))
	{
		vertexInfo.attributeDescriptions[index].binding = 0;
		vertexInfo.attributeDescriptions[index].location = index;
		vertexInfo.attributeDescriptions[index].format = VK_FORMAT_R32G32B32_SFLOAT;
		vertexInfo.attributeDescriptions[index].offset = vertexInfo.bindingDescription.stride;
		vertexInfo.bindingDescription.stride += sizeof(point3D);
		index++;
	}
	if (Bitmask::HasFlag(V, Coordinate))
	{
		vertexInfo.attributeDescriptions[index].binding = 0;
		vertexInfo.attributeDescriptions[index].location = index;
		vertexInfo.attributeDescriptions[index].format = VK_FORMAT_R32G32_SFLOAT;
		vertexInfo.attributeDescriptions[index].offset = vertexInfo.bindingDescription.stride;
		vertexInfo.bindingDescription.stride += sizeof(point2D);
		index++;
	}
	if (Bitmask::HasFlag(V, Normal))
	{
		vertexInfo.attributeDescriptions[index].binding = 0;
		vertexInfo.attributeDescriptions[index].location = index;
		vertexInfo.attributeDescriptions[index].format = VK_FORMAT_R32G32B32_SFLOAT;
		vertexInfo.attributeDescriptions[index].offset = vertexInfo.bindingDescription.stride;
		vertexInfo.bindingDescription.stride += sizeof(point3D);
		index++;
	}
	if (Bitmask::HasFlag(V, Color))
	{
		vertexInfo.attributeDescriptions[index].binding = 0;
		vertexInfo.attributeDescriptions[index].location = index;
		vertexInfo.attributeDescriptions[index].format = VK_FORMAT_R32G32B32_SFLOAT;
		vertexInfo.attributeDescriptions[index].offset = vertexInfo.bindingDescription.stride;
		vertexInfo.bindingDescription.stride += sizeof(point3D);
		index++;
	}

	vertexInfo.floatCount = vertexInfo.bindingDescription.stride / sizeof(float);

	return (vertexInfo);
}

VERTEX_TEMPLATE
std::vector<float> Vertex<V>::GetData()
{
	size_t size = ((Bitmask::HasFlag(V, Position) ? 3 : 0) + (Bitmask::HasFlag(V, Coordinate) ? 2 : 0) +
		(Bitmask::HasFlag(V, Normal) ? 3 : 0) + (Bitmask::HasFlag(V, Color) ? 3 : 0));

	std::vector<float> data(size);

	uint32_t i = 0;

	if constexpr (Bitmask::HasFlag(V, Position))
	{
		data[i++] = this->position[0];
		data[i++] = this->position[1];
		data[i++] = this->position[2];
	}
	if constexpr (Bitmask::HasFlag(V, Coordinate))
	{
		data[i++] = this->coordinate[0];
		data[i++] = this->coordinate[1];
	}
	if constexpr (Bitmask::HasFlag(V, Normal))
	{
		data[i++] = this->normal[0];
		data[i++] = this->normal[1];
		data[i++] = this->normal[2];
	}
	if constexpr (Bitmask::HasFlag(V, Color))
	{
		data[i++] = this->color[0];
		data[i++] = this->color[1];
		data[i++] = this->color[2];
	}

	return (data);
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
	//out << mesh.GetVertex(0) << " ";

	out << std::endl;
	//out << "size: " << mesh.GetData().size() << std::endl;
	//for (const Vertex<V>& vertex : mesh.GetVertices()) { out << vertex << std::endl; };
	for (const float& val : mesh.GetData()) { out << val << " "; };

	return (out);
}