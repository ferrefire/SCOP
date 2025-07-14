#include "time.hpp"

void Time::Frame()
{
	SetFrameTimes();
}

void Time::SetFrameTimes()
{
	currentFrameTime = glfwGetTime();
	deltaFrameTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;
}

float Time::deltaFrameTime = 0.0f;
float Time::lastFrameTime = 0.0f;
float Time::currentFrameTime = 0.0f;

const float& Time::deltaTime = Time::deltaFrameTime;