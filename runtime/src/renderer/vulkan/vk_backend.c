#include "vk_backend.h"
#include "core/logger.h"
#include "core/memory.h"

#include "assets.h"
#include "vk_utils.h"
#include "vk_device.h"
#include "vk_swapchain.h"

typedef struct internal_state {
	VkInstance instance;
	VulkanDevice device;
	VkSurfaceKHR surface;
	VulkanSwapchain swapchain;
	MeshData mesh;
} InternalState;

b8 vulkan_backend_init(Window* window, BackendState* back_state) {
	VASSERT(back_state);
	back_state->internal_state = mem_alloc(sizeof(InternalState));
	InternalState* state = (InternalState*)back_state->internal_state;

	VkApplicationInfo app_info = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "VeloxEngine",
		.apiVersion = VK_API_VERSION_1_4,
	};

	u32 n_extensions = 0;
	const char** extensions = win_get_vk_instance_ext(&n_extensions);

	VkInstanceCreateInfo instance_info = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &app_info,
		.enabledExtensionCount = n_extensions,
		.ppEnabledExtensionNames = extensions,
	};
	VK_CHECK(vkCreateInstance(&instance_info, NULL, &state->instance));

	vk_device_create(state->instance, &state->device);

	win_create_vk_surface(window, state->instance, &state->surface);
	i32 width, height = 0;
	win_get_framebuffer_size(window, &width, &height);
	vk_swapchain_create(&state->device, width, height, state->surface, &state->swapchain);

	load_mesh_data("runtime/assets/mesh/suzanne.obj", &state->mesh); // TEMP
	return TRUE;
}

void vulkan_backend_shutdown(BackendState* back_state) {
	VASSERT(back_state);
	InternalState* state = (InternalState*)back_state->internal_state;
	vkDeviceWaitIdle(state->device.logical_dev);

	destroy_mesh_data(&state->mesh); // TEMP

	vk_swapchain_destroy(&state->device, &state->swapchain);
	vk_device_destroy(&state->device);

	if (state->instance != VK_NULL_HANDLE) {
		vkDestroyInstance(state->instance, NULL);
		state->instance = VK_NULL_HANDLE;
	}
}

void vulkan_backend_render_frame(Window* window, BackendState* back_state) {
	InternalState* state = (InternalState*)back_state->internal_state;
}
