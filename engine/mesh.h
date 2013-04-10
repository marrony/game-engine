/*
 * mesh.h
 *
 *  Created on: 09/04/2013
 *      Author: marrony
 */

#ifndef MESH_H_
#define MESH_H_

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

#endif /* MESH_H_ */
