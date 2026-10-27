#include "vk_backend.h"
#include "core/logger.h"
#include "core/memory.h"

#include "vk_device.h"
#include "vk_instance.h"
#include "vk_swapchain.h"

typedef struct internal_state {
	VulkanInstance instance;
	VulkanDevice device;
	VkSurfaceKHR surface;
	VulkanSwapchain swapchain;
} InternalState;

b8 vulkan_backend_init(Window* window, BackendState* back_state) {
	VASSERT(back_state);
	back_state->internal_state = mem_alloc(sizeof(InternalState));
	InternalState* state = (InternalState*)back_state->internal_state;

	vk_instance_create(&state->instance);
	vk_device_create(&state->instance, &state->device);

	win_create_vk_surface(window, state->instance.handle, &state->surface);
	i32 width, height = 0;
	win_get_framebuffer_size(window, &width, &height);
	vk_swapchain_create(&state->device, width, height, state->surface, &state->swapchain);

	return TRUE;
}

void vulkan_backend_shutdown(BackendState* back_state) {
	VASSERT(back_state);
	InternalState* state = (InternalState*)back_state->internal_state;
	vkDeviceWaitIdle(state->device.logical_dev);

	vk_swapchain_destroy(&state->device, &state->swapchain);
	vk_device_destroy(&state->device);
	vk_instance_destroy(&state->instance);
}

void vulkan_backend_render_frame(Window* window, BackendState* back_state) {
	InternalState* state = (InternalState*)back_state->internal_state;
}
