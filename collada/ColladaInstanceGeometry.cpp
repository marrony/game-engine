/*
 * ColladaInstanceGeometry.cpp
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#include "ColladaInstanceGeometry.h"
#include "ColladaBindMaterial.h"
#include "ColladaDocument.h"
#include "ColladaUtil.h"

DEFINE_VISITOR(ColladaInstanceGeometry)

ColladaInstanceGeometry::~ColladaInstanceGeometry() {
	delete bindMaterial;
}

std::string ColladaInstanceGeometry::elementType() {
	return "instance_geometry";
}

void ColladaInstanceGeometry::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	ColladaUtil::loadAttribute(element, "url", url);
	bindMaterial = ColladaUtil::createElementFromXml<ColladaBindMaterial>(this, element);
}
