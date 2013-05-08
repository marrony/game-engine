/*
 * CreateGeometry.cpp
 *
 *  Created on: 21/05/2012
 *      Author: marrony
 */

#include "CreateGeometry.h"
#include "ColladaInput.h"
#include "ColladaArray.h"

#include "mesh.h"
#include "mesh_io.h"

CreateGeometry::CreateGeometry() {
}

CreateGeometry::~CreateGeometry() {
}

void CreateGeometry::visit(ColladaGeometry* geometry) {
	if(!geometry->getName().empty())
		name = geometry->getName();
	else
		name = geometry->getId();

	ColladaMesh* mesh = geometry->getMesh();

	mesh->accept(this);

	save_mesh();
}

void CreateGeometry::visit(ColladaMesh* mesh) {
	const std::vector<ColladaPolyList*>& polylist = mesh->getPolylists();
	for(size_t i = 0; i < polylist.size(); i++)
		polylist[i]->accept(this);

	const std::vector<ColladaTriangles*>& triangle = mesh->getTriangles();
	for(size_t i = 0; i < triangle.size(); i++)
		triangle[i]->accept(this);
}

struct VertexSoup {
	size_t icount;
	const std::vector<int>& primitive;
	std::vector<MeshVertex> vertices;
	std::vector<unsigned short> indices;
	ColladaInput* vertexInput;
	ColladaInput* normalInput;
	ColladaInput* texCoordInput;
	ColladaSource* position;
	ColladaSource* normal;
	ColladaSource* texCoord;
	int inputs;
	int flags;

	VertexSoup(ColladaGeometricPrimitive* g, const std::vector<int>& primitive)
			: primitive(primitive) {
		icount = 0;
		texCoord = 0;
		normal = 0;

		vertexInput = g->findInputWithSemantic("VERTEX");
		normalInput = g->findInputWithSemantic("NORMAL");
		texCoordInput = g->findInputWithSemantic("TEXCOORD");

		inputs = -1;
		flags = MeshVertex::POSITION;

		position = vertexInput->getSourceElement();
		inputs = std::max(inputs, vertexInput->getOffset());

		if(normalInput) {
			normal = normalInput->getSourceElement();
			inputs = std::max(inputs, normalInput->getOffset());
			flags |= MeshVertex::NORMAL;
		}

		if(texCoordInput) {
			texCoord = texCoordInput->getSourceElement();
			inputs = std::max(inputs, texCoordInput->getOffset());
			flags |= MeshVertex::TEXTURE;
		}

		inputs++;
	}

	struct Find {
		const MeshVertex vertex;
		int flags;
		bool normalInput;
		bool texCoordInput;

		Find(const MeshVertex& vertex, int flags, bool normalInput, bool texCoordInput)
				: vertex(vertex), flags(flags), normalInput(normalInput), texCoordInput(texCoordInput) { }

		bool operator()(const MeshVertex other) const {
			bool equals = (vertex.position - other.position).length() <= 0.001;

			if(flags & MeshVertex::NORMAL)
				equals = equals && (normalInput && (vertex.normal - other.normal).length() <= 0.001);

			if(flags & MeshVertex::TEXTURE)
				equals = equals && (texCoordInput && (vertex.texCoord - other.texCoord).length() <= 0.001);

			return equals;
		}
	};

	int addVertex(const MeshVertex& vertex) {
		std::vector<MeshVertex>::iterator ite = std::find_if(vertices.begin(), vertices.end(), Find(vertex, flags, normalInput, texCoordInput));

		int index;

		if(ite == vertices.end()) {
			index = vertices.size();
			vertices.push_back(vertex);
		} else {
			index = std::distance(vertices.begin(), ite);
		}

		return index;
	}

	bool hasVertex() {
		return (icount+inputs-1) < primitive.size();
	}

	MeshVertex nextVertex() {
		MeshVertex vertex;

		int v = primitive[icount + vertexInput->getOffset()];
		vertex.position.x = position->getFloatArray()->getArray()[v*3 + 0];
		vertex.position.y = position->getFloatArray()->getArray()[v*3 + 1];
		vertex.position.z = position->getFloatArray()->getArray()[v*3 + 2];

		if(normalInput) {
			int n = primitive[icount + normalInput->getOffset()];
			vertex.normal.x = normal->getFloatArray()->getArray()[n*3 + 0];
			vertex.normal.y = normal->getFloatArray()->getArray()[n*3 + 1];
			vertex.normal.z = normal->getFloatArray()->getArray()[n*3 + 2];
		}

		if(texCoordInput) {
			int t = primitive[icount + texCoordInput->getOffset()];
			vertex.texCoord.x = texCoord->getFloatArray()->getArray()[t*2 + 0];
			vertex.texCoord.y = texCoord->getFloatArray()->getArray()[t*2 + 1];
		}

		icount += inputs;

		return vertex;
	}
};

void CreateGeometry::visit(ColladaPolyList* polylist) {
	const std::vector<std::vector<int> >& primitive = polylist->getPrimitive();
	for(size_t i = 0; i < primitive.size(); i++) {
		VertexSoup vertexSoup(polylist, primitive[i]);

		const std::vector<int>& vcount = polylist->getVcount();
		for(size_t x = 0; x < vcount.size(); x++) {
			int vc = vcount[x];
			int index[3];

			if(!vertexSoup.hasVertex()) break;
			index[0] = vertexSoup.addVertex(vertexSoup.nextVertex());

			if(!vertexSoup.hasVertex()) break;
			index[1] = vertexSoup.addVertex(vertexSoup.nextVertex());

			if(!vertexSoup.hasVertex()) break;
			index[2] = vertexSoup.addVertex(vertexSoup.nextVertex());

			vertexSoup.indices.insert(vertexSoup.indices.end(), index, index + 3);

			for(int i = 3; i < vc; i++) {
				index[1] = index[2];

				if(!vertexSoup.hasVertex()) break;
				index[2] = vertexSoup.addVertex(vertexSoup.nextVertex());

				vertexSoup.indices.insert(vertexSoup.indices.end(), index, index + 3);
			}
		}

		add_vertex_data(vertexSoup.vertices, vertexSoup.indices, polylist->getMaterial(), vertexSoup.flags);
	}
}

void CreateGeometry::visit(ColladaTriangles* triangles) {
	const std::vector<std::vector<int> >& primitive = triangles->getPrimitive();
	for(size_t i = 0; i < primitive.size(); i++) {
		VertexSoup vertexSoup(triangles, primitive[i]);

		while(vertexSoup.hasVertex()) {
			int index = vertexSoup.addVertex(vertexSoup.nextVertex());
			vertexSoup.indices.push_back(index);
		}

		add_vertex_data(vertexSoup.vertices, vertexSoup.indices, triangles->getMaterial(), vertexSoup.flags);
	}
}

void CreateGeometry::add_vertex_data(const std::vector<MeshVertex>& vertexArray, const std::vector<uint16_t>& newIndices, const std::string& material, int flags) {
	size_t lastVertexCount = position.size();

	size_t material_index;
	std::vector<std::string>::iterator mat = std::find(materials.begin(), materials.end(), material);
	if(mat == materials.end()) {
		material_index = materials.size();
		materials.push_back(material);
	} else
		material_index = std::distance(materials.begin(), mat);

	Batch batch;
	batch.offset = indices.size();
	batch.count = newIndices.size();
	batch.start = lastVertexCount + *std::min_element(newIndices.begin(), newIndices.end());
	batch.end = lastVertexCount + *std::max_element(newIndices.begin(), newIndices.end());
	batches.push_back(batch);

	for(size_t i = 0; i < newIndices.size(); i++) {
		indices.push_back(lastVertexCount + newIndices[i]);
	}

	size_t vertexCount = lastVertexCount + vertexArray.size();

	position.resize(vertexCount);
	for(size_t i = 0; i < newIndices.size(); i++) {
		size_t index = newIndices[i];

		position[lastVertexCount + index] = vertexArray[index].position;
	}

	if(flags & MeshVertex::NORMAL) {
		normal.resize(vertexCount);
		for(size_t i = 0; i < newIndices.size(); i++) {
			size_t index = newIndices[i];

			normal[lastVertexCount + index] = vertexArray[index].normal;
		}
	}

	if(flags & MeshVertex::TEXTURE) {
		texCoord.resize(vertexCount);
		for(size_t i = 0; i < newIndices.size(); i++) {
			size_t index = newIndices[i];

			texCoord[lastVertexCount + index] = vertexArray[index].texCoord;
		}
	}

	if(flags & MeshVertex::sTANGENT) {
		sTangent.resize(vertexCount);
		for(size_t i = 0; i < newIndices.size(); i++) {
			size_t index = newIndices[i];

			sTangent[lastVertexCount + index] = vertexArray[index].sTangent;
		}
	}

	if(flags & MeshVertex::tTANGENT) {
		tTangent.resize(vertexCount);
		for(size_t i = 0; i < newIndices.size(); i++) {
			size_t index = newIndices[i];

			tTangent[lastVertexCount + index] = vertexArray[index].tTangent;
		}
	}

	if(flags & MeshVertex::COLOR) {
		color.resize(vertexCount);
		for(size_t i = 0; i < newIndices.size(); i++) {
			size_t index = newIndices[i];

			color[lastVertexCount + index] = vertexArray[index].color;
		}
	}

	if(flags & MeshVertex::BONES) {
		boneIds.resize(vertexCount);
		weights.resize(vertexCount);

		for(size_t i = 0; i < newIndices.size(); i++) {
			size_t index = newIndices[i];

			boneIds[lastVertexCount + index] = vertexArray[index].boneIds;
			weights[lastVertexCount + index] = vertexArray[index].weights;
		}
	}
}

void CreateGeometry::save_mesh() {
	int32_t attributeOffsets[Mesh::MaxAttributes];

	for(int i = 0; i < Mesh::MaxAttributes; i++)
		attributeOffsets[i] = -1;

	size_t offset = indices.size() * sizeof(uint16_t);

	attributeOffsets[Mesh::Vertex] = offset;
	offset += position.size() * Mesh::get_stride(Mesh::Vertex);

	if(!normal.empty()) {
		attributeOffsets[Mesh::Normal] = offset;
		offset += position.size() * Mesh::get_stride(Mesh::Normal);
	}

	if(!sTangent.empty() && !tTangent.empty()) {
		attributeOffsets[Mesh::STangent] = offset;
		offset += position.size() * Mesh::get_stride(Mesh::STangent);

		attributeOffsets[Mesh::TTangent] = offset;
		offset += position.size() * Mesh::get_stride(Mesh::TTangent);
	}

	if(!color.empty()) {
		attributeOffsets[Mesh::Color] = offset;
		offset += position.size() * Mesh::get_stride(Mesh::Color);
	}

	if(!texCoord.empty()) {
		attributeOffsets[Mesh::TexCoord] = offset;
		offset += position.size() * Mesh::get_stride(Mesh::TexCoord);
	}

	if(!boneIds.empty() && !weights.empty()) {
		attributeOffsets[Mesh::BoneIds] = offset;
		offset += position.size() * Mesh::get_stride(Mesh::BoneIds);

		attributeOffsets[Mesh::Weigths] = offset;
		offset += position.size() * Mesh::get_stride(Mesh::Weigths);
	}

	int32_t batch_offset = offset;
	offset += batches.size() * sizeof(Batch);

	Mesh* mesh = (Mesh*)malloc(sizeof(Mesh) + offset);

	mesh->batch_offset = batch_offset;
	mesh->batch_count = (uint16_t)batches.size();
	mesh->vertex_count = (uint16_t)position.size();
	mesh->index_count = (uint16_t)indices.size();

	memcpy(mesh->offsets, attributeOffsets, sizeof(attributeOffsets));
	memcpy(mesh->index_pointer(), indices.data(), mesh->index_count*sizeof(uint16_t));
	memcpy(mesh->batches_pointer(), batches.data(), mesh->batch_count*sizeof(Batch));
	memcpy(mesh->get_pointer(Mesh::Vertex), position.data(), mesh->vertex_count*Mesh::get_stride(Mesh::Vertex));

	if(mesh->offsets[Mesh::Normal] != -1)
		memcpy(mesh->get_pointer(Mesh::Normal), normal.data(), mesh->vertex_count*Mesh::get_stride(Mesh::Normal));

	if(mesh->offsets[Mesh::STangent] != -1)
		memcpy(mesh->get_pointer(Mesh::STangent), sTangent.data(), mesh->vertex_count*Mesh::get_stride(Mesh::STangent));

	if(mesh->offsets[Mesh::TTangent] != -1)
		memcpy(mesh->get_pointer(Mesh::TTangent), tTangent.data(), mesh->vertex_count*Mesh::get_stride(Mesh::TTangent));

	if(mesh->offsets[Mesh::Color] != -1)
		memcpy(mesh->get_pointer(Mesh::Color), color.data(), mesh->vertex_count*Mesh::get_stride(Mesh::Color));

	if(mesh->offsets[Mesh::TexCoord] != -1)
		memcpy(mesh->get_pointer(Mesh::TexCoord), texCoord.data(), mesh->vertex_count*Mesh::get_stride(Mesh::TexCoord));

	if(mesh->offsets[Mesh::BoneIds] != -1)
		memcpy(mesh->get_pointer(Mesh::BoneIds), boneIds.data(), mesh->vertex_count*Mesh::get_stride(Mesh::BoneIds));

	if(mesh->offsets[Mesh::Weigths] != -1)
		memcpy(mesh->get_pointer(Mesh::Weigths), weights.data(), mesh->vertex_count*Mesh::get_stride(Mesh::Weigths));

	mesh_write("teste.mesh", mesh);
	mesh_destroy(mesh);
}
