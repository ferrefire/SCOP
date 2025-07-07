#include "manager.hpp"

#include "graphics.hpp"
#include "renderer.hpp"
#include "descriptor.hpp"

#include "pipeline.hpp"
#include "mesh.hpp"
#include "buffer.hpp"
#include "pass.hpp"
#include "bitmask.hpp"
#include "command.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <iostream>
#include <utility>

Manager::Manager()
{

}

Manager::~Manager()
{
	Destroy();
}

void Manager::Create()
{
	try
	{
		CreateGLFW();
		CreateVulkan();
	}
	catch(const std::exception& e)
	{
		throw (std::runtime_error(e.what()));
	}
}

void Manager::CreateGLFW()
{
	if (!glfwInit()) throw (std::runtime_error("Failed to initiate GLFW"));

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	window.CreateFrame();
}

void Manager::CreateVulkan()
{
	Graphics::CreateInstance();
	device.CreatePhysical();
	window.CreateSurface(device);
	std::cout << "Window created: " << window << std::endl;
	device.SelectQueues();
	device.CreateLogical();
	device.RetrieveQueues();
	std::cout << "Device created: " << device << std::endl;
	swapchain.Create(&window, &device);
	std::cout << "Swapchain created: " << swapchain << std::endl;
	Renderer::Create(swapchain.GetFrameCount(), &device, &swapchain);
}

void Manager::Destroy()
{
	if (device.IsCreated() && vkDeviceWaitIdle(device.GetLogicalDevice()) != VK_SUCCESS)
		throw (std::runtime_error("Failed to wait for device to be idle"));

	DestroyGLFW();
	DestroyVulkan();
}

void Manager::DestroyGLFW()
{
	window.DestroyFrame();
	glfwTerminate();
}

void Manager::DestroyVulkan()
{
	swapchain.Destroy();
	Renderer::Destroy();
	window.DestroySurface();
	device.Destroy();
	Graphics::DestroyInstance();
}

Window& Manager::GetWindow()
{
	return (window);
}

Device& Manager::GetDevice()
{
	return (device);
}

Swapchain& Manager::GetSwapchain()
{
	return (swapchain);
}

void Manager::Start()
{
	//Buffer buffer;
	//BufferConfig bufferConfig{};
	//bufferConfig.size = 16;
	//buffer.Create(bufferConfig, &device);

	//Pass pass;
	//PassConfig passConfig = Pass::DefaultConfig();
	//pass.Create(passConfig, &device);

	//Pipeline pipeline;
	//PipelineConfig config = Pipeline::DefaultConfig();
	//config.shader = "screen";
	//config.vertexInfo = Mesh::GetVertexInfo(true, false, false, false);
	//config.renderpass = pass.GetRenderpass();
	//pipeline.Create(config, &device);

	//Command command;
	//CommandConfig commandConfig{};
	//commandConfig.queueIndex = device.GetQueueIndex(QueueType::Graphics);
	//command.Create(commandConfig, &device);

	//Mesh<Position, VK_INDEX_TYPE_UINT16> mesh;
	//float val = 0;
	//for (int i = 0; i < 3; i++)
	//{
	//	Vertex<Position> vert;
	//	vert.position = point3D({val, val + 1, val + 2});
	//	val += 3;
	//	//vert.coordinate = point2D({val, val + 1});
	//	//val += 2;
	//	//vert.normal = point3D({val, val + 1, val + 2});
	//	//val += 3;
	//	mesh.AddVertex(vert);
	//	mesh.AddIndex(i);
	//}
	//mesh.Create(&device);
	//std::cout << "Mesh created: " << mesh << std::endl;
	//auto& vert = mesh.NewVertex();
	//vert.normal = point3D({22, 32, 1234});
	//Vertex<Position | Normal> vertex;
	//vertex.position = point3D({1, 2, 3});
	////vertex.coordinate = point2D({4, 5});
	//vertex.normal = point3D({6, 7, 8});
	////vertex.color = point3D({9, 10, 11});
	//std::cout << "Mesh created: " << mesh << std::endl;
}

void Manager::Frame()
{
	glfwPollEvents();

	Renderer::Frame();
}

bool Manager::ShouldClose()
{
	return (glfwWindowShouldClose(window.GetData()));
}

Window Manager::window;
Device Manager::device;
Swapchain Manager::swapchain;