#pragma once
#include "defines.h"
#include "vk_instance.h"
#include <vulkan/vulkan_core.h>

typedef struct vulkan_device {
  VkPhysicalDevice phys_dev;
  VkDevice logical_dev;
  VkQueue gfx_queue;
	VkFormat depth_format;
} VulkanDevice;

void vk_device_create(VulkanInstance *instance, VulkanDevice *out_device);
void vk_device_destroy(VulkanDevice *device);

i32 find_memory_index(VulkanDevice* device, u32 type_filter, u32 property_flags);
