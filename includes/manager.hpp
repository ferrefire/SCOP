#pragma once

#include "window.hpp"
#include "device.hpp"
#include "swapchain.hpp"

class Manager
{
	private:
		static Window window;
		static Device device;
		static Swapchain swapchain;

		static void CreateGLFW();
		static void CreateVulkan();

		static void DestroyGLFW();
		static void DestroyVulkan();

	public:
		Manager();
		~Manager();

		static void Create();
		
		static void Destroy();

		static Window& GetWindow();
		static Device& GetDevice();
		static Swapchain& GetSwapchain();

		static void Start();
		static void Frame();
		
		static bool ShouldClose();
};