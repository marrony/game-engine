/*
 * ColladaLight.cpp
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#include "ColladaLight.h"

DEFINE_VISITOR(ColladaLight)

std::string ColladaLight::elementType() {
	return "light";
}

void ColladaLight::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	//TODO parse light
}
