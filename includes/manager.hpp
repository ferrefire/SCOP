#pragma once

#include "window.hpp"
#include "device.hpp"

class Manager
{
	private:
		static Window window;
		static Device device;

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
};