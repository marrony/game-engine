#include "app.h"

#include <cstring>
#include <cctype>
#include <string.h>
#include <stdint.h>

struct Mesh {
	uint16_t index_offset;
	uint16_t vertex_offset;
	uint16_t color_offset;
	uint16_t vertex_count;
	uint16_t index_count;
	uint8_t data[0];

	unsigned short* index_pointer() const {
		return (unsigned short*)(data + index_offset);
	}

	float* vertex_pointer() const {
		return (float*)(data + vertex_offset);
	}

	float* color_pointer() const {
		return (float*)(data + color_offset);
	}
};

class Game2 : public Game {
	Application& app;
	Mesh* mesh;
public:
	Game2(Application& app) : app(app), mesh(0) {
	}

	virtual void initialize() {
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
		mesh = (Mesh*)malloc(sizeof(Mesh) + size);

		mesh->index_offset = 0;
		mesh->vertex_offset = index_size;
		mesh->color_offset = index_size + vertex_size;
		mesh->vertex_count = vertex_size / sizeof(float) / 3;
		mesh->index_count = index_size / sizeof(uint16_t);

		memcpy(mesh->index_pointer(), indices, index_size);
		memcpy(mesh->vertex_pointer(), vertices, vertex_size);
		memcpy(mesh->color_pointer(), colors, color_size);
	}

	virtual void finalize() {
		free(mesh);
	}

	virtual void doFrame() {
		if (app.isKeyPressed(KEY_ESCAPE)) {
			app.stopMainLoop();
			return;
		}

		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-1., 1., -1., 1., 1., 20.);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, mesh->vertex_pointer());
		glColorPointer(3, GL_FLOAT, 0, mesh->color_pointer());
		glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_SHORT, mesh->index_pointer());
	}
};

enum Type {
	TP_INVALID,
	TP_STRING,
	TP_NUMBER,
	TP_OBJECT,
	TP_ARRAY,
	TP_TRUE,
	TP_FALSE,
	TP_NULL,
};

struct Pair;

struct Value {
	Type type;
	union {
		const char* string;
		float number;
		Pair* object;
		Value* array;
	};
	int size;
};

struct Pair {
	const char* name;
	Value value;
};

void printIdent(int ident) {
	for(int i = 0; i < ident; i++)
		fprintf(stderr, "\t");
}

void print(const Value* value, int ident) {
	switch(value->type) {
	case TP_STRING:
		fprintf(stderr, "\"%s\"", value->string);
		break;
	case TP_NUMBER:
		fprintf(stderr, "%f", value->number);
		break;
	case TP_OBJECT:
		fprintf(stderr, "{\n");
		for(int i = 0; i < value->size; i++) {
			printIdent(ident+1); fprintf(stderr, "\"%s\": ", value->object[i].name);
			print(&value->object[i].value, ident+1);
			if(i+1 < value->size)
				fprintf(stderr, ",\n");
		}
		fprintf(stderr, "\n");
		printIdent(ident); fprintf(stderr, "}");
		break;
	case TP_ARRAY:
		fprintf(stderr, "[\n");
		for(int i = 0; i < value->size; i++) {
			printIdent(ident+1); print(&value->array[i], ident+1);
			if(i+1 < value->size)
				fprintf(stderr, ",\n");
		}
		fprintf(stderr, "\n");
		printIdent(ident); fprintf(stderr, "]");
		break;
	case TP_TRUE:
		fprintf(stderr, "true");
		break;
	case TP_FALSE:
		fprintf(stderr, "false");
		break;
	case TP_NULL:
		fprintf(stderr, "null");
		break;
	}
};

int parseNumber(const char* data, int begin, int end) {
	int old_begin = begin;
	while(begin < end && isalnum(data[begin]))
		begin++;
	return begin;
}

int parseString(const char* data, int begin, int end) {
	int old_begin = begin;
	begin++;
	while(begin < end && data[begin] == '\"')
		begin++;
	if(data[begin] == '\"')
		begin++;
	return begin;
}

int parseValue(const char* data, int begin, int end) {
	if(isalnum(data[begin])) {
		begin = parseNumber(data, begin, end);
	}
	return begin;
}

void ignoreSpace(const char* data, int* begin, int end) {
	while(*begin < end && isspace(data[*begin]))
		(*begin)++;
}

int nextToken(const char* data, int begin, int end, int* new_begin) {
	int old_begin = begin;

	while(begin < end) {
		if(isalnum(data[begin])) {
			while(begin < end && isalnum(data[begin]))
				begin++;

			*new_begin = begin;
			return TP_NUMBER;
		} else if(data[begin] == '\"') {
			begin++;
			while(begin < end && data[begin] != '\"')
				begin++;

			*new_begin = begin;
			return TP_STRING;
		} else if(data[begin] == ',') {
			*new_begin = begin+1;
			return ',';
		} else if(data[begin] == '{') {
			*new_begin = begin+1;
			return '{';
		} else if(data[begin] == '[') {
			*new_begin = begin+1;
			return '[';
		} else if(data[begin] == ':') {
			*new_begin = begin+1;
			return ':';
		} else if(begin+4 < end && data[begin] == 't') {
			*new_begin = begin+4;
			return TP_TRUE;
		} else if(begin+5 < end && data[begin] == 'f') {
			*new_begin = begin+5;
			return TP_FALSE;
		} else if(begin+4 < end && data[begin] == 'n') {
			*new_begin = begin+4;
			return TP_NULL;
		} else {
			begin++;
		}
	}

	return TP_INVALID;
}

int parseArray(const char* data, int begin, int end) {
	begin++;

	int old_begin = begin;
	while(begin < end) {
		if(isalnum(data[begin])) {
			begin = parseValue(data, begin, end);
		} else if(isspace(data[begin])) {
			begin++;
		} else if(data[begin] == ',') {
			begin++;
		} else if(data[begin] == ']') {
			begin++;
			break;
		}
	}

	return begin;
}

int parseObject(const char* data, int begin, int end) {
	begin++;

	int old_begin = begin;
	while(begin < end) {
		if(data[begin] == '\"') {
			begin = parseString(data, begin, end);
			ignoreSpace(data, &begin, end);
			if(data[begin] != ':')
				break;
			begin = parseValue(data, begin, end);
		} else if(isspace(data[begin])) {
			begin++;
		} else if(data[begin] == ',') {
			begin++;
		} else if(data[begin] == '}') {
			begin++;
			break;
		}
	}

	return begin;
}

Value* parseJson(const char* data, int lenght) {
	int begin = 0;
	int new_begin;

	int token;
	while((token = nextToken(data, begin, lenght, &new_begin)) != TP_INVALID) {
		fprintf(stderr, "%d: %d-%d\n", token, begin, new_begin);
		begin = new_begin;
	}

	while(begin < lenght) {
		if(data[begin] == '[') {
			begin = parseArray(data, begin, lenght);
		} else if(data[begin] == '{') {
			begin = parseObject(data, begin, lenght);
		} else {
			begin++;
		}
	}

	return NULL;
}

int main(void) {
	char data[] = "[10, 10]";
	parseJson(data, sizeof(data));

	Value* v = new Value;
	v->type = TP_ARRAY;
	v->size = 2;
	v->array = new Value[3];

	v->array[0].type = TP_OBJECT;
	v->array[0].object = new Pair[2];
	v->array[0].size = 2;
	v->array[0].object[0].name = "firstName";
	v->array[0].object[0].value.type = TP_STRING;
	v->array[0].object[0].value.string = "firstName";
	v->array[0].object[1].name = "lastName";
	v->array[0].object[1].value.type = TP_STRING;
	v->array[0].object[1].value.string = "lastName";

	v->array[1].type = TP_OBJECT;
	v->array[1].object = new Pair[3];
	v->array[1].size = 3;
	v->array[1].object[0].value.type = TP_FALSE;
	v->array[1].object[0].name = "firstName";
	v->array[1].object[1].value.type = TP_TRUE;
	v->array[1].object[1].name = "lastName";
	v->array[1].object[2].value.type = TP_NULL;
	v->array[1].object[2].name = "teste";

	print(v, 0);
	return 0;

	Application app("my x11/win32 window", 600, 600, true);
	Game2 game(app);
	return app.run(game);
}

