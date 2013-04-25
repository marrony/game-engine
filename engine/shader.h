/*
 * shader.h
 *
 *  Created on: 24/04/2013
 *      Author: marrony
 */

#ifndef SHADER_H_
#define SHADER_H_

#include <vector>

#include <stdint.h>

enum ShaderType {
	VertexShader,
	FragmentShader,
	GeometryShader
};

struct Source {
	const char* source;
	ShaderType type;
};

class ShaderSystem {
	struct Shader {
		const char* name;
		const char* filename;
		uint32_t id;
	};

	std::vector<Shader> shaders;
public:
	int32_t create_shader(const char* name, size_t source_count, const Source* sources);
	int32_t load_shader(const char* filename);
	int32_t find_shader(const char* name);
	void destroy_shader(int32_t shader);
};

#endif /* SHADER_H_ */
