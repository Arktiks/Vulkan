#pragma once

#include <vulkan\vulkan.h>

class Renderer
{
public:
	Renderer();
	~Renderer();

private:
	void InitInstance();
	void DeInstance();

	void InitDevice();
	void DeDevice();

	VkInstance                  instance         = nullptr;
	VkPhysicalDevice            gpu              = nullptr;
	VkDevice                    device           = nullptr;
	VkPhysicalDeviceProperties  gpu_properties   = {};

	uint32_t           graphics_family_index = 0;
};

