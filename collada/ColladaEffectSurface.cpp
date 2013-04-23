/*
 * ColladaEffectSurface.cpp
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#include "ColladaEffectSurface.h"

DEFINE_VISITOR(ColladaInitFrom)

std::string ColladaInitFrom::elementType() {
	return "init_from";
}

void ColladaInitFrom::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	const char* v = element->Value();
	if(v)
		value = v;
}

DEFINE_VISITOR(ColladaEffectSurface)

ColladaEffectSurface::~ColladaEffectSurface() {
	delete initFrom;
}

std::string ColladaEffectSurface::elementType() {
	return "surface";
}

void ColladaEffectSurface::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	ColladaUtil::loadAttribute(element, "type", type);

	initFrom = ColladaUtil::createElementFromXml<ColladaInitFrom>(this, element);
}
