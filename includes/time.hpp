#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Time
{
	private:
		static float deltaFrameTime;
		static float lastFrameTime;
		static float currentFrameTime;

		static void SetFrameTimes();

	public:
		static const float& deltaTime;

		static void Frame();
};