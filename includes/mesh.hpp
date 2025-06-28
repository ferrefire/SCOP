#pragma once

#include "buffer.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct MeshConfig
{
	bool positions = true;
	bool coordinates = false;
	bool normals = false;
	bool colors = false;
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
};