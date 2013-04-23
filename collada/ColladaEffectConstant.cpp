/*
 * ColladaEffectConstant.cpp
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#include "ColladaEffectConstant.h"

DEFINE_VISITOR(ColladaEffectConstant)

std::string ColladaEffectConstant::elementType() {
	return "constant";
}

void ColladaEffectConstant::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);
}
