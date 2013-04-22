/*
 * ColladaInstanceEffect.cpp
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#include "ColladaInstanceEffect.h"
#include "ColladaDocument.h"

DEFINE_VISITOR(ColladaInstanceEffect)

std::string ColladaInstanceEffect::elementType() {
	return "instance_effect";
}

void ColladaInstanceEffect::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	loadAttribute(element, "url", url);
}
