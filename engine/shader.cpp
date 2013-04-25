/*
 * shader.cpp
 *
 *  Created on: 24/04/2013
 *      Author: marrony
 */

#include "shader.h"

#include <string.h>

int32_t ShaderSystem::create_shader(const char* name, size_t source_count, const Source* sources) {
	Shader shader;
	shader.name = name;
	shader.filename = NULL;
	shader.id = 0;

	size_t index;
	for(index = 0; index < shaders.size(); index++) {
		if(!shaders[index].id)
			break;
	}

	if(index >= shaders.size())
		shaders.push_back(shader);
	else
		shaders[index] = shader;

	return (int32_t)index;
}

int32_t ShaderSystem::load_shader(const char* filename) {
	std::vector<Source> sources;

	return create_shader("", sources.size(), sources.data());
}

int32_t ShaderSystem::find_shader(const char* name) {
	for(size_t index = 0; index < shaders.size(); index++) {
		if(!strcmp(shaders[index].name, name))
			return (int32_t)index;
	}
	return -1;
}

void ShaderSystem::destroy_shader(int32_t shader) {
	if(shader < shaders.size() && shaders[shader].id) {
	}
}
