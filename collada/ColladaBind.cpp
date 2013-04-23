/*
 * ColladaBind.cpp
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#include "ColladaBind.h"
#include "ColladaUtil.h"

DEFINE_VISITOR(ColladaBind)

std::string ColladaBind::elementType() {
	return "bind";
}

void ColladaBind::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	ColladaUtil::loadAttribute(element, "semantic", semantic);
	ColladaUtil::loadAttribute(element, "target", target);
}
