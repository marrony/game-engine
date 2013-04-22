/*
 * ColladaBind.cpp
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#include "ColladaBind.h"

#include "tinyxml.h"

DEFINE_VISITOR(ColladaBind)

std::string ColladaBind::elementType() {
	return "bind";
}

void ColladaBind::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	loadAttribute(element, "semantic", semantic);
	loadAttribute(element, "target", target);
}
