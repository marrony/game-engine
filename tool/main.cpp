#include "app.h"

#if 0
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

	mesh->index_offset = 0;
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

//		glClearColor(1.0, 1.0, 1.0, 1.0);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		glMatrixMode(GL_PROJECTION);
//		glLoadIdentity();
//		glOrtho(-1., 1., -1., 1., 1., 20.);
//
//		glMatrixMode(GL_MODELVIEW);
//		glLoadIdentity();
//		//gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);
//
//		glEnableClientState(GL_VERTEX_ARRAY);
//		glEnableClientState(GL_COLOR_ARRAY);
//
//		glVertexPointer(3, GL_FLOAT, 0, mesh->vertex_pointer());
//		glColorPointer(3, GL_FLOAT, 0, mesh->color_pointer());
//		glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_SHORT, mesh->index_pointer());
#endif

int main(int argc, char* argv[]) {
	Application app("my x11/win32 window", 600, 600, true);
	return app.run();
}

