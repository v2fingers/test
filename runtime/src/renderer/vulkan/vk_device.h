#pragma once
#include "defines.h"
#include "vma/vk_mem_alloc.h"
#include <vulkan/vulkan_core.h>

typedef struct vulkan_device {
  VkPhysicalDevice physical_dev;
  VkDevice logical_dev;
  VkQueue gfx_queue;
	VkFormat depth_format;
  VmaAllocator allocator;
} VulkanDevice;

void vk_device_create(VkInstance instance, VulkanDevice *out_device);
void vk_device_destroy(VulkanDevice *device);

i32 find_memory_index(VulkanDevice* device, u32 type_filter, u32 property_flags);
