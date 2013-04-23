/*
 * ColladaGeometry.cpp
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#include "ColladaGeometry.h"
#include "ColladaMesh.h"
#include "ColladaUtil.h"

DEFINE_VISITOR(ColladaGeometry)

ColladaGeometry::~ColladaGeometry() {
	delete mesh;
}

std::string ColladaGeometry::elementType() {
	return "geometry";
}

void ColladaGeometry::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	TiXmlElement* geometryChild;

	geometryChild = element->FirstChildElement("convex_mesh");
	if(geometryChild != 0) {
		//throw Exception("Does not support <convex_mesh>");
	}

	geometryChild = element->FirstChildElement("spline");
	if(geometryChild != 0) {
		//throw Exception("Does not support <spline>");
	}

	mesh = ColladaUtil::createElementFromXml<ColladaMesh>(this, element);
}
