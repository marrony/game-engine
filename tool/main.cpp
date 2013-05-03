#include "app.h"

#include "mesh.h"
#include "mesh_io.h"
#include "task.h"
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

	Batch batch;
	batch.offset = 0;
	batch.count = 2;
	batch.start = 0;
	batch.end = 3;
	batch.material = 0;

	int vertex_size = sizeof(vertices);
	int color_size = sizeof(colors);
	int index_size = sizeof(indices);
	int batch_size = sizeof(batch);
	int size = vertex_size + color_size + index_size + batch_size;
	Mesh* mesh = (Mesh*)malloc(sizeof(Mesh) + size);

	for(int i = 0; i < Mesh::MaxAttributes; i++)
		mesh->offsets[i] = -1;

	mesh->offsets[Mesh::Vertex] = index_size;
	mesh->offsets[Mesh::Color] = index_size + vertex_size;
	mesh->offsets[Mesh::Batches] = index_size + vertex_size + color_size;
	mesh->vertex_count = vertex_size / sizeof(float) / 3;
	mesh->index_count = index_size / sizeof(uint16_t);
	mesh->batch_count = 1;

	memcpy(mesh->index_pointer(), indices, index_size);
	memcpy(mesh->get_pointer(Mesh::Vertex), vertices, vertex_size);
	memcpy(mesh->get_pointer(Mesh::Color), colors, color_size);
	memcpy(mesh->get_pointer(Mesh::Batches), &batch, batch_size);

	return mesh;
}

void animation_function(TaskId task_id, WorkItem& item) {
	fprintf(stdout, "animation_function(%d) %lx\n", task_id, pthread_self());
	fflush(stdout);

	usleep((rand() % 10) * 10000);
}

WorkItem animation_task() {
	WorkItem item = {animation_function};
	return item;
}

void scene_graph_function(TaskId task_id, WorkItem& item) {
	fprintf(stdout, "scene_graph_function(%d) %lx\n", task_id, pthread_self());
	fflush(stdout);

	usleep((rand() % 10) * 10000);
}

WorkItem scene_graph_task() {
	WorkItem item = {scene_graph_function};
	return item;
}

void gui_function(TaskId task_id, WorkItem& item) {
	fprintf(stdout, "gui_function(%d) %lx\n", task_id, pthread_self());
	fflush(stdout);

	usleep((rand() % 10) * 10000);
}

WorkItem gui_task() {
	WorkItem item = {gui_function};
	return item;
}

void render_function(TaskId task_id, WorkItem& item) {
	fprintf(stdout, "render_function(%d) %lx\n", task_id, pthread_self());
	fflush(stdout);

	usleep((rand() % 10) * 10000);
}

WorkItem render_task() {
	WorkItem item = {render_function};
	return item;
}

void sound_update_function(TaskId task_id, WorkItem& item) {
	fprintf(stdout, "sound_update_function(%d) %lx\n", task_id, pthread_self());
	fflush(stdout);

	usleep((rand() % 10) * 10000);
}

WorkItem sound_update_task() {
	WorkItem item = {sound_update_function};
	return item;
}

void do_tasks(TaskManager* task_manager) {
	TaskId done = task_manager->begin_add_empty();

	TaskId animation = task_manager->add(animation_task());
	TaskId gui = task_manager->add(gui_task());
	TaskId sound = task_manager->add(sound_update_task());

	task_manager->add_child(done, sound);

	/*gui_scene*/ {
		TaskId gui_scene = task_manager->begin_add_empty();

		/*scene_graph*/ {
			TaskId scene_graph = task_manager->add(scene_graph_task(), animation);

			task_manager->add_child(gui_scene, scene_graph);
		} /*scene_graph*/

		task_manager->add_child(gui_scene, gui);

		task_manager->finish_add(gui_scene);

		/*render*/ {
			TaskId render = task_manager->add(render_task(), gui_scene);

			task_manager->add_child(done, render);
		} /*render*/

	} /*gui_scene*/

	task_manager->finish_add(done);

	task_manager->wait(done);
}

pthread_t ids[2];
pthread_mutex_t mutex;
pthread_cond_t buffer_not_empty;
pthread_cond_t buffer_not_full;

int buffer[10];
int size = 0;
int offset = 0;

void* producer(void* arg) {
	for(int i = 0; i < 100; i++) {
		pthread_mutex_lock(&mutex);

		while(size == 10)
			pthread_cond_wait(&buffer_not_full, &mutex);

		buffer[(offset + size) % 10] = i;
		size++;

		pthread_cond_signal(&buffer_not_empty);

		pthread_mutex_unlock(&mutex);
	}

	return NULL;
}

void* consumer(void* arg) {
	while(true) {
		pthread_mutex_lock(&mutex);

		while(size == 0)
			pthread_cond_wait(&buffer_not_empty, &mutex);

		fprintf(stdout, "consumer: %p, item: %d\n", arg, buffer[offset]);
		fflush(stdout);

		size--;
		offset = (offset + 1) % 10;

		pthread_cond_signal(&buffer_not_full);

		pthread_mutex_unlock(&mutex);
	}

	return NULL;
}

void test_function(TaskId task_id, WorkItem& item) {
	fprintf(stdout, "test_function(%d) %lx\n", task_id, pthread_self());
	fflush(stdout);

	usleep(10000);
}

WorkItem test_task() {
	WorkItem item = {test_function};
	return item;
}

TaskManager* task_manager;

void inifinity_function(TaskId task_id, WorkItem& item) {
	fprintf(stdout, "inifinity_function(%d) %lx\n", task_id, pthread_self());
	fflush(stdout);

	usleep(10000);

	TaskId test = task_manager->begin_add(test_task());
	task_manager->finish_add(test);

	task_manager->wait(test);
}

WorkItem inifinity_task() {
	WorkItem item = {inifinity_function};
	return item;
}

void test_task_manager() {
	task_manager = new TaskManager(32);

	fprintf(stdout, "main thread: %lx\n", pthread_self());
	fflush(stdout);

	for (int i = 0; i < 100000; i++) {
		TaskId infinity = task_manager->add(inifinity_task());

		do_tasks(task_manager);

		task_manager->wait(infinity);

		fprintf(stdout, "finish: %d\n", i);
		fflush(stdout);
	}

	fprintf(stdout, "finished\n");
}

int main(int argc, char* argv[]) {
	Mesh* mesh = create_mesh();
	mesh_write("quad.mesh", mesh);
	mesh_destroy(mesh);

	Application app("my x11/win32 window", 600, 600, true);
	return app.run();
}
