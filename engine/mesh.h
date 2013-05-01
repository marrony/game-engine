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

struct Batch {
	uint16_t offset; //offset in indices array
	uint16_t count;  //number os elements to draw
	uint16_t start;  //minimum index in range [offset, offset+count]
	uint16_t end;    //maximum index in range [offset, offset+count]
	uint16_t material;
};

struct Mesh {
	enum Attributes {
		Batches,
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
	uint16_t vertex_count;
	uint16_t index_count;
	uint16_t batch_count;
	uint8_t data[0];

	uint16_t* index_pointer() const {
		return (uint16_t*)data;
	}

	float* get_pointer(Attributes attribute) const {
		return (float*)(data + offsets[attribute]);
	}

	static inline int16_t get_stride(Attributes attribute) {
		static int16_t stride[] = {
				sizeof(Batch),
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
		static int16_t size[] = {0, 3, 3, 3, 3, 3, 2, 4, 4};
		return size[attribute];
	}

	Batch* batches_pointer() const {
		return (Batch*)(data + offsets[Batches]);
	}

	int16_t material_count() const {
		int16_t count = -1;

		Batch* batches = batches_pointer();
		for(int16_t i = 0; i < batch_count; i++) {
			if(batches[i].material > count)
				count = batches[i].material;
		}

		return count+1;
	}

	uint32_t sizeof_mesh() const {
		uint32_t size = sizeof(uint16_t)*index_count;

		size += offsets[Batches] != -1 ? get_stride(Batches)*batch_count : 0;
		for(int i = 1; i < MaxAttributes; i++)
			size += offsets[i] != -1 ? get_stride((Attributes)i)*vertex_count : 0;

		return sizeof(Mesh) + size;
	}
};

static inline void mesh_destroy(Mesh* mesh) {
	free(mesh);
}

#endif /* MESH_H_ */
