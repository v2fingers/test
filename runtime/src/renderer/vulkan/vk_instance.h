#pragma once
#include "defines.h"
#include <vulkan/vulkan_core.h>

typedef struct vulkan_instance {
  VkInstance handle;
  const char** layers;
  const char** exts;
  u32 n_layers, n_exts;
} VulkanInstance;

void vk_instance_create(VulkanInstance *out_instance);
void vk_instance_destroy(VulkanInstance *instance);
