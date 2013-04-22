/*
 * ColladaMaterial.cpp
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#include "ColladaMaterial.h"
#include "ColladaInstanceEffect.h"
#include "ColladaImage.h"
#include "ColladaEffect.h"

#include "tinyxml.h"

DEFINE_VISITOR(ColladaMaterial)

ColladaMaterial::~ColladaMaterial() {
	delete instanceEffect;
}

std::string ColladaMaterial::elementType() {
	return "material";
}

void ColladaMaterial::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	instanceEffect = createElementFromXml<ColladaInstanceEffect>(element);
}
