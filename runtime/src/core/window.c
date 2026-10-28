#include "window.h"
#include "core/logger.h"
#include "core/memory.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct window {
	GLFWwindow* handle;
};

Window* win_init(u32 width, u32 height, char* title) {
	Window* out_window = mem_alloc(sizeof(struct window));

	glfwInit();
	VASSERT_MSG(glfwVulkanSupported(), "Vulkan is not supported on this system");

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	out_window->handle = glfwCreateWindow(width, height, title, NULL, NULL);

	VASSERT(out_window->handle);
	return out_window;
}

void win_shutdown(Window* window) {
	VASSERT(window);

	glfwDestroyWindow(window->handle);
	glfwTerminate();
	mem_free(window, sizeof(struct window));
}

b8 win_should_close(const Window* window) {
	return glfwWindowShouldClose(window->handle);
}

void win_get_framebuffer_size(Window* window, i32* out_width, i32* out_height) {
	glfwGetFramebufferSize(window->handle, out_width, out_height);
}

void win_poll_events(void) {
	glfwPollEvents();
}

b8 win_create_vk_surface(Window* window, VkInstance instance, VkSurfaceKHR* out_surface) {
	return glfwCreateWindowSurface(instance, window->handle, NULL, out_surface);
}

const char** win_get_vk_instance_ext(u32* count) {
	return glfwGetRequiredInstanceExtensions(count);
}

b8 win_check_vk_present_support(VkInstance instance, VkPhysicalDevice device, i16 queue_fam) {
	return glfwGetPhysicalDevicePresentationSupport(instance, device, queue_fam);
}
