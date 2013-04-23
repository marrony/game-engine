/*
 * ColladaSkeleton.cpp
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#include "ColladaSkeleton.h"

DEFINE_VISITOR(ColladaSkeleton)

std::string ColladaSkeleton::elementType() {
	return "skeleton";
}

void ColladaSkeleton::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	uri = element->GetText();
}
