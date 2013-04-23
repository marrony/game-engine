/*
 * ColladaInstanceMaterial.cpp
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#include "ColladaInstanceMaterial.h"
#include "ColladaMaterial.h"
#include "ColladaDocument.h"
#include "ColladaUtil.h"

DEFINE_VISITOR(ColladaInstanceMaterial)

std::string ColladaInstanceMaterial::elementType() {
	return "instance_material";
}

void ColladaInstanceMaterial::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	ColladaUtil::loadAttribute(element, "target", target);
	ColladaUtil::loadAttribute(element, "symbol", symbol);
}
