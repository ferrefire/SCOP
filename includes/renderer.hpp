#pragma once

#include "command.hpp"
#include "pass.hpp"
#include "device.hpp"
#include "swapchain.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class Renderer
{
	private:
		static Device* device;
		static Swapchain* swapchain;

		static uint32_t frameCount;
		static uint32_t currentFrame;
		static uint32_t renderIndex;

		static std::vector<VkFence> fences;
		static std::vector<VkSemaphore> renderSemaphores;
		static std::vector<VkSemaphore> presentSemaphores;
		static std::vector<Command> commands;
		static std::vector<Pass*> passes;

		static void CreateFences();
		static void CreateSemaphores();
		static void CreateCommands();

		static void RecordCommands();
		static void PresentFrame();

	public:
		Renderer();
		~Renderer();

		static void Create(uint32_t rendererFrameCount, Device* rendererDevice, Swapchain* rendererSwapchain);

		static void Destroy();

		static void Frame();

		static void AddPass(Pass* pass);
};