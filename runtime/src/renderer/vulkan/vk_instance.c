#include "vk_instance.h"
#include "core/window.h"
#include "vk_utils.h"

void vk_instance_create(VulkanInstance* out_instance) {
	VkApplicationInfo app_info = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "VeloxEngine",
		.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
		.pEngineName = "Velox",
		.engineVersion = VK_MAKE_VERSION(1, 0, 0),
		.apiVersion = VK_API_VERSION_1_4,
	};
	out_instance->exts = win_get_vk_instance_ext(&out_instance->n_exts);

	VkInstanceCreateInfo instance_info = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &app_info,
		.enabledExtensionCount = out_instance->n_exts,
		.ppEnabledExtensionNames = out_instance->exts,
	};
	VK_CHECK(vkCreateInstance(&instance_info, NULL, &out_instance->handle));
}

void vk_instance_destroy(VulkanInstance* instance) {
	if (instance->handle != VK_NULL_HANDLE) {
		vkDestroyInstance(instance->handle, NULL);
		instance->handle = VK_NULL_HANDLE;
	}
}
