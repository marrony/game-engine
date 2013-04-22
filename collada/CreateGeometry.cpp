/*
 * CreateGeometry.cpp
 *
 *  Created on: 21/05/2012
 *      Author: marrony
 */

#include "CreateGeometry.h"
#include "ColladaArray.h"

#include "Model.h"
#include "Material.h"
#include "ResourceManager.h"

CreateGeometry::CreateGeometry(const std::string& name, ResourceManager* manager) :
		manager(manager), geometryData(0), name(name) {
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

	geometryData = new Mesh;

	geometryData->setName(name0);

	ColladaMesh* mesh = geometry->getMesh();

	mesh->accept(this);
}

void CreateGeometry::visit(ColladaMesh* mesh) {
	for(ColladaPolyList* polylist : mesh->getPolylists())
		polylist->accept(this);

	for(ColladaTriangles* triangle : mesh->getTriangles())
		triangle->accept(this);
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

	VertexSoup(ColladaGeometricPrimitive* g, const std::vector<int>& primitive) : primitive(primitive) {
		icount = 0;

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

	int addVertex(const MeshVertex& vertex) {
		std::vector<MeshVertex>::iterator ite = std::find_if(vertices.begin(), vertices.end(), [&](const MeshVertex& other) {
			bool equals = (vertex.position == other.position);

			if(flags & MeshVertex::NORMAL)
				equals &= (normalInput && vertex.normal == other.normal);

			if(flags & MeshVertex::TEXTURE)
				equals &= (texCoordInput && vertex.texCoord == other.texCoord);

			return equals;
		});

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
	for(const std::vector<int>& p : polylist->getPrimitive()) {
		VertexSoup vertexSoup(polylist, p);

		for(int vc : polylist->getVcount()) {
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

		Material* material = (Material*)manager->loadResource(MaterialKey(polylist->getMaterial()));
		geometryData->addVertexData(vertexSoup.vertices, vertexSoup.indices, material, vertexSoup.flags);
	}
}

void CreateGeometry::visit(ColladaTriangles* triangles) {
	for(const std::vector<int>& p : triangles->getPrimitive()) {
		VertexSoup vertexSoup(triangles, p);

		while(vertexSoup.hasVertex()) {
			int index = vertexSoup.addVertex(vertexSoup.nextVertex());
			vertexSoup.indices.push_back(index);
		}

		Material* material = (Material*)manager->loadResource(MaterialKey(triangles->getMaterial()));
		geometryData->addVertexData(vertexSoup.vertices, vertexSoup.indices, material, vertexSoup.flags);
	}
}
