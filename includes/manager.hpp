#pragma once

#include "window.hpp"

class Manager
{
	private:
		static Window window;

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