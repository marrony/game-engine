/*
 * shader.cpp
 *
 *  Created on: 24/04/2013
 *      Author: marrony
 */

#include "shader.h"
#include "opengl.h"

#include <string.h>
#include <stdio.h>

static uint32_t compile_shader(const Source& source) {
	GLenum glType;

	switch(source.type) {
	case VertexShader:
		glType = GL_VERTEX_SHADER;
		break;

	case FragmentShader:
		glType = GL_FRAGMENT_SHADER;
		break;

	case GeometryShader:
		glType = GL_GEOMETRY_SHADER;
		break;

	default:
		return (uint32_t)-1;
	}

	uint32_t shader_id = glCreateShader(glType);
	const char* src = source.source;
	glShaderSource(shader_id, 1, &src, 0);
	glCompileShader(shader_id);

	int status = 0;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);

	if(!status) {
		int lenght;
		char log[1024];

		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &lenght);
		glGetShaderInfoLog(shader_id, 1024, &lenght, log);

		printf("%s\n", log);
	}

	return shader_id;
}

int32_t ShaderSystem::create_shader(const char* name, size_t source_count, const Source* sources) {
	Shader shader;
	shader.name = name;
	shader.filename = NULL;
	shader.id = glCreateProgram();

	for(size_t i = 0; i < source_count; i++) {
		uint32_t shader_id = compile_shader(sources[i]);
		glAttachShader(shader.id, shader_id);
		glDeleteShader(shader_id);
	}

	glLinkProgram(shader.id);

	int status = 0;
	glGetProgramiv(shader.id, GL_LINK_STATUS, &status);
	if(!status) {
		int lenght;
		char log[1024];

		glGetProgramiv(shader.id, GL_INFO_LOG_LENGTH, &lenght);
		glGetProgramInfoLog(shader.id, 1024, &lenght, log);

		printf("%s\n", log);
	}

	int numAttribs = 0;
	glGetProgramiv(shader.id, GL_ACTIVE_ATTRIBUTES, &numAttribs);

	shader.attributes.resize(numAttribs);
	for(int i = 0; i < numAttribs; i++) {
		GLchar name[256] = { 0 };
		GLint size = 0;
		GLenum type = 0;

		glGetActiveAttrib(shader.id, i, sizeof(name), NULL, &size, &type, name);

		shader.attributes[i].name = name;
		shader.attributes[i].index = glGetAttribLocation(shader.id, name);
		shader.attributes[i].size = size;
		shader.attributes[i].type = type;
	}

	int numUniforms = 0;
	glGetProgramiv(shader.id, GL_ACTIVE_UNIFORMS, &numUniforms);

	shader.uniforms.resize(numUniforms);
	for(int i = 0; i < numUniforms; i++) {
		GLchar name[256] = { 0 };
		GLint size = 0;
		GLenum type = 0;

		glGetActiveUniform(shader.id, i, sizeof(name), NULL, &size, &type, name);
		int uniform = glGetUniformLocation(shader.id, name);
		shader.uniforms[i].name = name;
		shader.uniforms[i].index = glGetUniformLocation(shader.id, name);
		shader.uniforms[i].size = size;
		shader.uniforms[i].type = type;
	}

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
	if(shader >= 0 && shader < shaders.size() && shaders[shader].id) {
		glDeleteProgram(shaders[shader].id);
		shaders[shader].id = 0;
	}
}

void ShaderSystem::bind_shader(int32_t shader) {
	bool is_valid = shader >= 0 && shader < shaders.size() && shaders[shader].id;
	glUseProgram(is_valid ? shaders[shader].id : 0);
}
