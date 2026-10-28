#include "vk_device.h"
#include "core/window.h"
#include "vk_utils.h"

void vk_device_create(VkInstance instance, VulkanDevice* out_device) {
	// Pick physical device
	// Temporary for now, need to expand for
	// - extention support
	// - discrete GPU
	u32 n_phys_devs = 0;
	VK_CHECK(vkEnumeratePhysicalDevices(instance, &n_phys_devs, NULL));
	VkPhysicalDevice devices[n_phys_devs];
	VK_CHECK(vkEnumeratePhysicalDevices(instance, &n_phys_devs, devices));
	out_device->physical_dev = devices[0];
	VkPhysicalDeviceProperties2 device_props = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
	};
	vkGetPhysicalDeviceProperties2(out_device->physical_dev, &device_props);
	VLOG_INFO("Selected device: %s", device_props.properties.deviceName);

	// Find graphics queue family
	u32 n_queue_fams = 0;
	vkGetPhysicalDeviceQueueFamilyProperties2(out_device->physical_dev, &n_queue_fams, NULL);
	VkQueueFamilyProperties2 queue_fam_props[n_queue_fams];
	for (u32 i = 0; i < n_queue_fams; i++) {
		queue_fam_props[i] = (VkQueueFamilyProperties2){
			.sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2,
			.pNext = NULL,
		};
	}
	vkGetPhysicalDeviceQueueFamilyProperties2(out_device->physical_dev, &n_queue_fams, queue_fam_props);
	u32 gfx_queue_fam = 0;
	for (usize i = 0; i < n_queue_fams; i++) {
		if (queue_fam_props[i].queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			gfx_queue_fam = i;
			break;
		}
	}
	win_check_vk_present_support(instance, out_device->physical_dev, gfx_queue_fam);

	// Create logical device
	const f32 qf_priorities = 1.0f;
	VkDeviceQueueCreateInfo queue_info = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.queueFamilyIndex = gfx_queue_fam,
		.queueCount = 1,
		.pQueuePriorities = &qf_priorities,
	};
	const char* device_exts[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

	VkPhysicalDeviceVulkan12Features enabled_vk12_features = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
		.descriptorIndexing = TRUE,
		.shaderSampledImageArrayNonUniformIndexing = TRUE,
		.descriptorBindingVariableDescriptorCount = TRUE,
		.runtimeDescriptorArray = TRUE,
		.bufferDeviceAddress = TRUE,
	};
	VkPhysicalDeviceVulkan13Features enabled_vk13_features = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
		.pNext = &enabled_vk12_features,
		.synchronization2 = TRUE,
		.dynamicRendering = TRUE,
	};
	VkPhysicalDeviceFeatures enabled_vk10_features = {
		.samplerAnisotropy = VK_TRUE,
	};

	VkDeviceCreateInfo device_info = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pNext = &enabled_vk13_features,
		.queueCreateInfoCount = 1,
		.pQueueCreateInfos = &queue_info,
		.enabledExtensionCount = VARRAY_COUNT(device_exts),
		.ppEnabledExtensionNames = device_exts,
		.pEnabledFeatures = &enabled_vk10_features,
	};
	VK_CHECK(vkCreateDevice(out_device->physical_dev, &device_info, NULL, &out_device->logical_dev));
	vkGetDeviceQueue(out_device->logical_dev, gfx_queue_fam, 0, &out_device->gfx_queue);

	// TODO: Device depth format checking
	out_device->depth_format = VK_FORMAT_D32_SFLOAT;

	// VMA init
	VmaVulkanFunctions vk_functions = {
		.vkGetInstanceProcAddr = vkGetInstanceProcAddr,
		.vkGetDeviceProcAddr = vkGetDeviceProcAddr,
		.vkCreateImage = vkCreateImage,
	};
	VmaAllocatorCreateInfo vma_info = {
		.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
		.physicalDevice = out_device->physical_dev,
		.device = out_device->logical_dev,
		.pVulkanFunctions = &vk_functions,
		.instance = instance,
	};
	VK_CHECK(vmaCreateAllocator(&vma_info, &out_device->allocator));
}

void vk_device_destroy(VulkanDevice* device) {
	if (device->logical_dev != VK_NULL_HANDLE) {
		vkDestroyDevice(device->logical_dev, NULL);
		device->logical_dev = VK_NULL_HANDLE;
	}
}
