/*
 * ColladaEffectSampler2D.cpp
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#include "ColladaEffectSampler2D.h"

DEFINE_VISITOR(ColladaEffectSampler2D)

std::string ColladaEffectSampler2D::elementType() {
	return "sampler2D";
}

void ColladaEffectSampler2D::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);
}
