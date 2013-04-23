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

	float* color_pointer() const {
		return (float*)(data + color_offset);
	}

	uint32_t sizeof_mesh() const {
		return sizeof(Mesh) + vertex_count*sizeof(float)*3*2 + index_count*sizeof(uint16_t);
	}
};

void mesh_destroy(Mesh* mesh) {
	free(mesh);
}

#endif /* MESH_H_ */
