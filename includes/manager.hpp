#pragma once

#include "window.hpp"
#include "device.hpp"

class Manager
{
	private:
		static Window window;
		static Device device;

	public:
		Manager();
		~Manager();

		static void Create();
		static void CreateGLFW();
		static void CreateVulkan();

		static void Destroy();
		static void DestroyGLFW();
		static void DestroyVulkan();
};