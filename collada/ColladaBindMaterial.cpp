/*
 * ColladaBindMaterial.cpp
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#include "ColladaBindMaterial.h"
#include "ColladaInstanceMaterial.h"
#include "ColladaUtil.h"

DEFINE_VISITOR(ColladaTechniqueCommon)

ColladaTechniqueCommon::~ColladaTechniqueCommon() {
	ColladaUtil::eraseVector(instanceMaterial);
}

std::string ColladaTechniqueCommon::elementType() {
	return "technique_common";
}

void ColladaTechniqueCommon::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	instanceMaterial = ColladaUtil::createElementsFromXml<ColladaInstanceMaterial>(this, element);
}

DEFINE_VISITOR(ColladaBindMaterial)

ColladaBindMaterial::~ColladaBindMaterial() {
	delete techniqueCommon;
}

std::string ColladaBindMaterial::elementType() {
	return "bind_material";
}

void ColladaBindMaterial::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	techniqueCommon = ColladaUtil::createElementFromXml<ColladaTechniqueCommon>(this, element);
}

