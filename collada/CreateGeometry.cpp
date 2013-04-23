/*
 * CreateGeometry.cpp
 *
 *  Created on: 21/05/2012
 *      Author: marrony
 */

#include "CreateGeometry.h"
#include "ColladaInput.h"
#include "ColladaArray.h"

CreateGeometry::CreateGeometry(const std::string& name) : name(name) {
	elementsPerVertex = 0;
}

CreateGeometry::~CreateGeometry() {
}

void CreateGeometry::visit(ColladaGeometry* geometry) {
	std::string name0;

	if(!name.empty())
		name0 = name;
	else if(!geometry->getName().empty())
		name0 = geometry->getName();
	else
		name0 = geometry->getId();

	ColladaMesh* mesh = geometry->getMesh();

	mesh->accept(this);
}

void CreateGeometry::visit(ColladaMesh* mesh) {
	for(size_t i = 0; i < mesh->getPolylists().size(); i++)
		mesh->getPolylists()[i]->accept(this);

	for(size_t i = 0; i < mesh->getTriangles().size(); i++)
		mesh->getTriangles()[i]->accept(this);
}

struct VertexSoup {
	size_t icount;
	const std::vector<int>& primitive;
	std::vector<MeshVertex> vertices;
	std::vector<unsigned short> indices;
	ColladaInput* vertexInput;
	ColladaInput* normalInput;
	ColladaInput* texCoordInput;
	ColladaElement* vertex;
	ColladaSource* position;
	ColladaSource* normal;
	ColladaSource* texCoord;
	int inputs;
	int flags;

	VertexSoup(ColladaGeometricPrimitive* g, const std::vector<int>& primitive)
			: primitive(primitive) {
		icount = 0;
		vertex = 0;
		texCoord = 0;

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
		const MeshVertex& vertex;
		int flags;
		bool normalInput;
		bool texCoordInput;

		Find(const MeshVertex& vertex, int flags, bool normalInput, bool texCoordInput)
				: vertex(vertex), flags(flags), normalInput(normalInput), texCoordInput(texCoordInput) { }

		bool operator()(const MeshVertex& other) const {
			bool equals = (vertex.position == other.position);

			if(flags & MeshVertex::NORMAL)
				equals &= (normalInput && vertex.normal == other.normal);

			if(flags & MeshVertex::TEXTURE)
				equals &= (texCoordInput && vertex.texCoord == other.texCoord);

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
	for(size_t i = 0; i < polylist->getPrimitive().size(); i++) {
		VertexSoup vertexSoup(polylist, polylist->getPrimitive()[i]);

		for(size_t x = 0; x < polylist->getVcount().size(); x++) {
			int vc = polylist->getVcount()[x];
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

		addVertexData(vertexSoup.vertices, vertexSoup.indices, 0, vertexSoup.flags);

//		Material* material = (Material*)manager->loadResource(MaterialKey(polylist->getMaterial()));
//		geometryData->addVertexData(vertexSoup.vertices, vertexSoup.indices, material, vertexSoup.flags);
	}
}

void CreateGeometry::visit(ColladaTriangles* triangles) {
	for(size_t i = 0; i < triangles->getPrimitive().size(); i++) {
		VertexSoup vertexSoup(triangles, triangles->getPrimitive()[i]);

		while(vertexSoup.hasVertex()) {
			int index = vertexSoup.addVertex(vertexSoup.nextVertex());
			vertexSoup.indices.push_back(index);
		}

		addVertexData(vertexSoup.vertices, vertexSoup.indices, 0, vertexSoup.flags);

//		Material* material = (Material*)manager->loadResource(MaterialKey(triangles->getMaterial()));
//		geometryData->addVertexData(vertexSoup.vertices, vertexSoup.indices, material, vertexSoup.flags);
	}
}

void CreateGeometry::addVertexData(const std::vector<MeshVertex>& vertexArray, const std::vector<unsigned short>& newIndices, int material, int flags) {
	size_t lastVertexCount = position.size();

	Batch indexMesh;
	indexMesh.offset = indices.size();
	indexMesh.count = newIndices.size();
	indexMesh.start = lastVertexCount + *std::min_element(newIndices.begin(), newIndices.end());
	indexMesh.end = lastVertexCount + *std::max_element(newIndices.begin(), newIndices.end());
	//batches.push_back(indexMesh);
	//materials.push_back(material);

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
