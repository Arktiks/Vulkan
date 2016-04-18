#include "Renderer.h"
#include <cassert>
#include <vector>

// TODO: Read on queue operations and queue families.

Renderer::Renderer()
{
	InitInstance();
	InitDevice();
}

Renderer::~Renderer()
{
	DeInstance();
	DeDevice();
}

void Renderer::InitInstance()
{
	// Structure to contain information about our application.
	VkApplicationInfo application_info{};
	application_info.sType                  = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	application_info.apiVersion             = VK_API_VERSION_1_0;
	application_info.applicationVersion     = VK_MAKE_VERSION(0, 1, 0);
	application_info.pApplicationName       = "Vulkan Rendering";

	// Structure to contain information about the instance we are creating.
	VkInstanceCreateInfo instance_create_info{};
	instance_create_info.sType              = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_create_info.pApplicationInfo   = &application_info;

	// Create Vulkan instance and test for success.
	VkResult error = vkCreateInstance(&instance_create_info, nullptr, &instance);
	assert(error == VK_SUCCESS && "[Renderer] vkCreateInstance failed.");
}

void Renderer::DeInstance()
{
	vkDestroyInstance(instance, nullptr);
	instance = nullptr;
}

void Renderer::InitDevice()
{
	// Enumerate through GPUs accessible to Vulkan and fill a vector with the handles.
	uint32_t gpu_count = 0; // Amount of usable GPUs.
	vkEnumeratePhysicalDevices(instance, &gpu_count, nullptr);
	std::vector<VkPhysicalDevice> gpu_list(gpu_count);
	vkEnumeratePhysicalDevices(instance, &gpu_count, gpu_list.data());
	gpu = gpu_list[0]; // For testing purposes we will only use the first available GPU.
	vkGetPhysicalDeviceProperties(gpu, &gpu_properties); // Take note of GPU properties for further checks.

	// TODO: Documentation.
	uint32_t family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, nullptr);
	std::vector<VkQueueFamilyProperties> family_property_list(family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(gpu, &family_count, family_property_list.data());

	// Generally NVIDIA has only one queue family with 16 queues on it.
	// AMD has more families of which all don't support graphics.
	// And Intel with one family that has only one queue.
	// <Citation needed>

	// Find a queue family that supports graphics.
	bool graphics_bit_found = false;
	for (uint32_t i = 0; i < family_count; i++)
	{
		if (family_property_list[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			graphics_bit_found = true;
			graphics_family_index = i;
			break;
		}
	}
	assert(graphics_bit_found == true && "[Renderer] VK_QUEUE_GRAPHICS_BIT not found.");

	// TODO: Documentation.
	float queue_priorities[]{ 1.0f };
	VkDeviceQueueCreateInfo device_queue_create_info{};
	device_queue_create_info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	device_queue_create_info.queueFamilyIndex = graphics_family_index;
	device_queue_create_info.queueCount       = 1;
	device_queue_create_info.pQueuePriorities = queue_priorities;

	// TODO: Documentation.
	VkDeviceCreateInfo device_create_info{};
	device_create_info.sType                  = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_create_info.queueCreateInfoCount   = 1;
	device_create_info.pQueueCreateInfos      = &device_queue_create_info;

	// Create Vulkan device and test for success.
	VkResult error = vkCreateDevice(gpu, &device_create_info, nullptr, &device);
	assert(error == VK_SUCCESS && "[Renderer] vkCreateDevice failed.");
}

void Renderer::DeDevice()
{
	vkDestroyDevice(device, nullptr);
	device = nullptr;
}
