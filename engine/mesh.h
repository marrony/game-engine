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

#include <stdlib.h>

struct Batch {
	uint16_t offset; //offset in indices array
	uint16_t count;  //number os elements to draw
	uint16_t start;  //minimum index in range [offset, offset+count]
	uint16_t end;    //maximum index in range [offset, offset+count]
};

struct Mesh {
	enum Attributes {
		Vertex,
		Normal,
		STangent,
		TTangent,
		Color,
		TexCoord,
		BoneIds,
		Weigths,
		MaxAttributes
	};

	int32_t offsets[MaxAttributes];
	int32_t batch_offset;
	uint16_t batch_count;
	uint16_t vertex_count;
	uint16_t index_count;
	uint8_t data[0];

	uint16_t* index_pointer() const {
		return (uint16_t*)data;
	}

	float* vertex_pointer() const {
		return (float*)(data + index_size());
	}

	Batch* batches_pointer() const {
		return (Batch*)(data + batch_offset);
	}

	int32_t index_size() const {
		return sizeof(uint16_t)*index_count;
	}

	int32_t vertex_size() const {
		int32_t size = 0;
		for(int i = 0; i < MaxAttributes; i++)
			size += offsets[i] != -1 ? get_stride((Attributes)i)*vertex_count : 0;
		return size;
	}

	int32_t batches_size() const {
		return sizeof(Batch)*batch_count;
	}

	float* get_pointer(Attributes attribute) const {
		return (float*)(data + offsets[attribute]);
	}

	static inline int16_t get_stride(Attributes attribute) {
		static int16_t stride[] = {
				sizeof(float)*3,
				sizeof(float)*3,
				sizeof(float)*3,
				sizeof(float)*3,
				sizeof(float)*3,
				sizeof(float)*2,
				sizeof(float)*4,
				sizeof(float)*4
		};
		return stride[attribute];
	}

	static inline int16_t get_size(Attributes attribute) {
		static int16_t size[] = {3, 3, 3, 3, 3, 2, 4, 4};
		return size[attribute];
	}

	uint32_t sizeof_mesh() const {
		uint32_t size = index_size() + vertex_size() + batches_size();
		return sizeof(Mesh) + size;
	}
};

static inline void mesh_destroy(Mesh* mesh) {
	free(mesh);
}

#endif /* MESH_H_ */
