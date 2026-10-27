#include "vk_swapchain.h"
#include "core/logger.h"
#include "core/memory.h"
#include "vk_utils.h"

void vk_swapchain_create(VulkanDevice* device, u32 width, u32 height, VkSurfaceKHR surface, VulkanSwapchain* out_swapchain) {
	VASSERT(out_swapchain);

	VkSurfaceCapabilitiesKHR surface_caps = {};
	VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device->phys_dev, surface, &surface_caps));
	VkExtent2D swapchain_extent = surface_caps.currentExtent;
	if (surface_caps.currentExtent.width == 0xFFFFFFFF) {
		VkExtent2D new_extent = {
			.width = width,
			.height = height,
		};
		swapchain_extent = new_extent;
	}

	out_swapchain->image_format = VK_FORMAT_B8G8R8A8_SRGB;
	// Very barebones, need to expand for things like HDR
	VkSwapchainCreateInfoKHR swapchain_info = {
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = surface,
		.minImageCount = surface_caps.minImageCount,
		.imageFormat = out_swapchain->image_format,
		.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR,
		.imageExtent = {.width = swapchain_extent.width, .height = swapchain_extent.height},
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.presentMode = VK_PRESENT_MODE_MAILBOX_KHR,
	};
	VK_CHECK(vkCreateSwapchainKHR(device->logical_dev, &swapchain_info, NULL, &out_swapchain->handle));

	vkGetSwapchainImagesKHR(device->logical_dev, out_swapchain->handle, &out_swapchain->n_images, NULL);
	out_swapchain->images = mem_calloc(out_swapchain->n_images, sizeof(VkImage));
	VK_CHECK(vkGetSwapchainImagesKHR(device->logical_dev, out_swapchain->handle, &out_swapchain->n_images, out_swapchain->images));

	out_swapchain->image_views = mem_calloc(out_swapchain->n_images, sizeof(VkImageView));
	for (u32 i = 0; i < out_swapchain->n_images; i++) {
		VkImageViewCreateInfo view_info = {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = out_swapchain->images[i],
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = out_swapchain->image_format,
			.subresourceRange = {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.levelCount = 1,
				.layerCount = 1,
			}};
		VK_CHECK(vkCreateImageView(device->logical_dev, &view_info, NULL, &out_swapchain->image_views[i]));
	}

	vk_image_create(
		device,
		width,
		height,
		device->depth_format,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		TRUE,
		VK_IMAGE_ASPECT_DEPTH_BIT,
		&out_swapchain->depth_image);
}

void vk_swapchain_destroy(VulkanDevice* device, VulkanSwapchain* swapchain) {
	if (swapchain->handle != VK_NULL_HANDLE) {
		vkDestroySwapchainKHR(device->logical_dev, swapchain->handle, NULL);
		swapchain->handle = VK_NULL_HANDLE;
	}
	mem_free(swapchain->images, swapchain->n_images * sizeof(VkImage));
	mem_free(swapchain->image_views, swapchain->n_images * sizeof(VkImageView));
}
