/*
 * mesh.h
 *
 *  Created on: 09/04/2013
 *      Author: marrony
 */

#ifndef MESH_H_
#define MESH_H_

#include <stddef.h>
#include <stdint.h>

struct Mesh {
	int32_t vertex_offset;
	int32_t normal_offset;
	int32_t stangent_offset;
	int32_t ttangent_offset;
	int32_t color_offset;
	int32_t texcoord_offset;
	int32_t boneids_offset;
	int32_t weights_offset;
	uint16_t vertex_count;
	uint16_t index_count;
	uint8_t data[0];

	unsigned short* index_pointer() const {
		return (unsigned short*)data;
	}

	float* vertex_pointer() const {
		return (float*)(data + vertex_offset);
	}

	float* normal_pointer() const {
		return (float*)(data + normal_offset);
	}

	float* stangent_pointer() const {
		return (float*)(data + stangent_offset);
	}

	float* ttangent_pointer() const {
		return (float*)(data + ttangent_offset);
	}

	float* color_pointer() const {
		return (float*)(data + color_offset);
	}

	float* texcoord_pointer() const {
		return (float*)(data + texcoord_offset);
	}

	float* boneids_pointer() const {
		return (float*)(data + boneids_offset);
	}

	float* weights_pointer() const {
		return (float*)(data + weights_offset);
	}

	uint32_t sizeof_mesh() const {
		uint32_t size = sizeof(uint16_t)*index_count;

		size += vertex_offset != -1 ? sizeof(float)*3*vertex_count : 0;
		size += normal_offset != -1 ? sizeof(float)*3*vertex_count : 0;
		size += stangent_offset != -1 ? sizeof(float)*3*vertex_count : 0;
		size += ttangent_offset != -1 ? sizeof(float)*3*vertex_count : 0;
		size += color_offset != -1 ? sizeof(float)*3*vertex_count : 0;
		size += texcoord_offset != -1 ? sizeof(float)*2*vertex_count : 0;
		size += boneids_offset != -1 ? sizeof(float)*4*vertex_count : 0;
		size += weights_offset != -1 ? sizeof(float)*4*vertex_count : 0;

		return sizeof(Mesh) + size;
	}
};

static inline void mesh_destroy(Mesh* mesh) {
	free(mesh);
}

#endif /* MESH_H_ */
