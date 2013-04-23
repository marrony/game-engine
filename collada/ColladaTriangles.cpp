/*
 * ColladaTriangles.cpp
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#include "ColladaTriangles.h"

DEFINE_VISITOR(ColladaTriangles)

std::string ColladaTriangles::elementType() {
	return "triangles";
}

void ColladaTriangles::loadFromXml(TiXmlElement* element) {
	ColladaGeometricPrimitive::loadFromXml(element);
}
