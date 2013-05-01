/*
 * shader.h
 *
 *  Created on: 24/04/2013
 *      Author: marrony
 */

#ifndef SHADER_H_
#define SHADER_H_

#include <vector>
#include <string>

#include <stdint.h>
#include <stddef.h>

enum ShaderType {
	VertexShader,
	FragmentShader,
	GeometryShader
};

enum UniformSemantic {
	ModelViewMatrix,
	ProjectionMatrix,
	ModelMatrix,
	ViewMatrix,
	NormalMatrix,
	LightPosition,
};

struct Source {
	const char* source;
	ShaderType type;
};

struct Location {
	int32_t semantic;
	int32_t index;
	int32_t size;
	uint32_t type;
};

class ShaderSystem {
	struct Shader {
		const char* name;
		const char* filename;
		uint32_t id;
		std::vector<Location> attributes;
		std::vector<Location> uniforms;
	};

	std::vector<Shader> shaders;
public:
	int32_t create_shader(const char* name, size_t source_count, const Source* sources);
	int32_t load_shader(const char* filename);
	int32_t find_shader(const char* name);
	void destroy_shader(int32_t shader);

	void bind_shader(int32_t shader);

	std::vector<Location> get_uniforms(int32_t shader) {
		return shaders[shader].uniforms;
	}

	std::vector<Location> get_attributes(int32_t shader) {
		return shaders[shader].attributes;
	}
};

#endif /* SHADER_H_ */
