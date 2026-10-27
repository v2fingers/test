#pragma once
#include "defines.h"
#include "vk_device.h"
#include "vk_image.h"
#include <vulkan/vulkan_core.h>

typedef struct vulkan_swapchain {
	VkSurfaceKHR surface;
	VkSwapchainKHR handle;

  u32 n_images;
	VkFormat image_format;
	VkImage* images;
	VkImageView* image_views;

  VulkanImage depth_image;
} VulkanSwapchain;

void vk_swapchain_create(VulkanDevice* device, u32 width, u32 height, VkSurfaceKHR surface, VulkanSwapchain* out_swapchain);
void vk_swapchain_destroy(VulkanDevice* device, VulkanSwapchain* swapchain);
