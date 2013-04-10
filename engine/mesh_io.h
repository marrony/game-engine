/*
 * mesh_io.h
 *
 *  Created on: 10/04/2013
 *      Author: marrony
 */

#ifndef MESH_IO_H_
#define MESH_IO_H_

#include "mesh.h"

#include <stdio.h>
#include <sys/stat.h>

static inline Mesh* mesh_read(const char* filename) {
	struct stat st;

	FILE* fp = fopen(filename, "r");
	fstat(fileno(fp), &st);

	Mesh* mesh = (Mesh*)malloc(st.st_size);
	fread(mesh, st.st_size, 1, fp);

	return mesh;
}

static inline void mesh_write(const char* filename, const Mesh* mesh) {
	FILE* fp = fopen(filename, "w");
	fwrite(mesh, mesh->sizeof_mesh(), 1, fp);
	fclose(fp);
}

#endif /* MESH_IO_H_ */
