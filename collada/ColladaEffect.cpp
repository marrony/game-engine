/*
 * ColladaEffect.cpp
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#include "ColladaEffect.h"
#include "ColladaImage.h"
#include "ColladaEffectNewParam.h"
#include "ColladaEffectProfileCommon.h"
#include "ColladaUtil.h"

DEFINE_VISITOR(ColladaEffect)

ColladaEffect::~ColladaEffect() {
	ColladaUtil::eraseVector(images);
	ColladaUtil::eraseVector(newParams);
	delete profileCommon;
}

std::string ColladaEffect::elementType() {
	return "effect";
}

void ColladaEffect::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	images = ColladaUtil::createElementsFromXml<ColladaImage>(this, element);
	newParams = ColladaUtil::createElementsFromXml<ColladaNewParam>(this, element);
	profileCommon = ColladaUtil::createElementFromXml<ColladaEffectProfileCommon>(this, element);
}
