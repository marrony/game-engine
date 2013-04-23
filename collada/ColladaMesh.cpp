/*
 * ColladaMesh.cpp
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#include "ColladaMesh.h"

DEFINE_VISITOR(ColladaMesh)

ColladaMesh::~ColladaMesh() {
	ColladaUtil::eraseVector(sources);
	delete vertices;
	ColladaUtil::eraseVector(triangles);
	ColladaUtil::eraseVector(polylists);
}

std::string ColladaMesh::elementType() {
	return "mesh";
}

void ColladaMesh::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	sources = ColladaUtil::createElementsFromXml<ColladaSource>(this, element);
	vertices = ColladaUtil::createElementFromXml<ColladaVertices>(this, element);
	triangles = ColladaUtil::createElementsFromXml<ColladaTriangles>(this, element);
	polylists = ColladaUtil::createElementsFromXml<ColladaPolyList>(this, element);
}
