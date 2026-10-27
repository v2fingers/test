#include "data.h"
#include "core/logger.h"
#include "core/memory.h"
#include "fastobj/fast_obj.h"
#include "renderer/renderer_resources.h"

b8 load_mesh_data(const char* path, MeshData* out_mesh) {
	fastObjMesh* mesh = fast_obj_read(path);
	VASSERT(mesh != NULL);
	Vertex3D* vertices = mem_alloc(mesh->index_count * sizeof(Vertex3D));
	u16* indices = mem_alloc(mesh->index_count * sizeof(u16));

	for (u64 i = 0; i < mesh->index_count; i++) {
		fastObjIndex index = mesh->indices[i];

		vertices[i] = (Vertex3D){
			.position = {
				mesh->positions[index.p * 3],
				-mesh->positions[index.p * 3 + 1],
				mesh->positions[index.p * 3 + 2]},
			.normal = {mesh->normals[index.n * 3], -mesh->normals[index.n * 3 + 1], mesh->normals[index.n * 3 + 2]},
			.uv = {mesh->texcoords[index.t * 2], 1.0f - mesh->texcoords[index.t * 2 + 1]}};

		indices[i] = (u16)i;
	}

	fast_obj_destroy(mesh);

	*out_mesh = (MeshData){
		.vertices = vertices,
		.indices = indices,
		.vertex_count = mesh->index_count,
		.index_count = mesh->index_count};
	return TRUE;
}

void destroy_mesh_data(MeshData* mesh) {
	mem_free(mesh->vertices, mesh->vertex_count * sizeof(Vertex3D));
	mem_free(mesh->indices, mesh->index_count * sizeof(u16));
	*mesh = (MeshData){0};
}
