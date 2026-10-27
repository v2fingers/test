#pragma once
#include "defines.h"
#include <vulkan/vulkan_core.h>
#include "renderer/renderer_resources.h"

typedef struct MeshData {
	Vertex3D* vertices;
	u16* indices;
	u64 vertex_count;
	u64 index_count;
} MeshData;

b8 load_mesh_data(const char* path, MeshData* out_mesh);
void destroy_mesh_data(MeshData* mesh);
