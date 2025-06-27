#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Pass
{
	private:
		VkRenderPass renderpass = nullptr;

	public:
		Pass();
		~Pass();

		void Create();

		void Destroy();
};