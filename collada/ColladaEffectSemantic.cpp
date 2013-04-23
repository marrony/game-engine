/*
 * ColladaEffectSemantic.cpp
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#include "ColladaEffectSemantic.h"

DEFINE_VISITOR(ColladaEffectSemantic)

std::string ColladaEffectSemantic::elementType() {
	return "semantic";
}

void ColladaEffectSemantic::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	semantic = element->GetText();
}
