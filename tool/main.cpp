#include "app.h"

#include "mesh.h"
#include "mesh_io.h"
#include <string.h>

Mesh* create_mesh() {
	static const float vertices[] = {
			-.75, -.75, -1.0,
			.75, -.75, -1.0,
			.75, .75, -1.0,
			-.75, .75, -1.0,
	};

	static const float colors[] = {
			1, 0, 0,
			0, 1, 0,
			0, 0, 1,
			1, 0, 1,
	};

	static const unsigned short indices[] = {
			0, 1, 2,
			0, 2, 3
	};

	int vertex_size = sizeof(vertices);
	int color_size = sizeof(colors);
	int index_size = sizeof(indices);
	int size = vertex_size + color_size + index_size;
	Mesh* mesh = (Mesh*)malloc(sizeof(Mesh) + size);

	mesh->vertex_offset = index_size;
	mesh->color_offset = index_size + vertex_size;
	mesh->vertex_count = vertex_size / sizeof(float) / 3;
	mesh->index_count = index_size / sizeof(uint16_t);

	memcpy(mesh->index_pointer(), indices, index_size);
	memcpy(mesh->vertex_pointer(), vertices, vertex_size);
	memcpy(mesh->color_pointer(), colors, color_size);

	return mesh;
}

void destroy_mesh(Mesh* mesh) {
	free(mesh);
}

int main(int argc, char* argv[]) {
	Mesh* mesh = create_mesh();
	mesh_write("quad.mesh", mesh);
	destroy_mesh(mesh);

	Application app("my x11/win32 window", 600, 600, true);
	return app.run();
}

