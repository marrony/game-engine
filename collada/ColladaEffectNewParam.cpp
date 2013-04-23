/*
 * ColladaEffectNewParam.cpp
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#include "ColladaEffectNewParam.h"
#include "ColladaEffectSampler2D.h"
#include "ColladaEffectSurface.h"
#include "ColladaEffectSemantic.h"
#include "ColladaUtil.h"

DEFINE_VISITOR(ColladaNewParam)

ColladaNewParam::~ColladaNewParam() {
	delete sampler2D;
	delete surface;
	delete semantic;
}

std::string ColladaNewParam::elementType() {
	return "newparam";
}

void ColladaNewParam::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	surface = ColladaUtil::createElementFromXml<ColladaEffectSurface>(this, element);
	sampler2D = ColladaUtil::createElementFromXml<ColladaEffectSampler2D>(this, element);
	semantic = ColladaUtil::createElementFromXml<ColladaEffectSemantic>(this, element);
}
