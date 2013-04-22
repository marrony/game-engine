/*
 * ColladaInstanceGeometry.cpp
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#include "ColladaInstanceGeometry.h"
#include "ColladaBindMaterial.h"
#include "ColladaDocument.h"

DEFINE_VISITOR(ColladaInstanceGeometry)

ColladaInstanceGeometry::~ColladaInstanceGeometry() {
	delete bindMaterial;
}

std::string ColladaInstanceGeometry::elementType() {
	return "instance_geometry";
}

void ColladaInstanceGeometry::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	loadAttribute(element, "url", url);
	bindMaterial = createElementFromXml<ColladaBindMaterial>(element);
}
