/*
 * ColladaEffectProfileCommon.cpp
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#include "ColladaEffectProfileCommon.h"
#include "ColladaImage.h"
#include "ColladaEffectNewParam.h"
#include "ColladaEffectTechnique.h"
#include "ColladaUtil.h"

DEFINE_VISITOR(ColladaEffectProfileCommon)

ColladaEffectProfileCommon::~ColladaEffectProfileCommon() {
	ColladaUtil::eraseVector(images);
	ColladaUtil::eraseVector(newParams);
	delete technique;
}

std::string ColladaEffectProfileCommon::elementType() {
	return "profile_COMMON";
}

void ColladaEffectProfileCommon::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	images = ColladaUtil::createElementsFromXml<ColladaImage>(this, element);
	newParams = ColladaUtil::createElementsFromXml<ColladaNewParam>(this, element);
	technique = ColladaUtil::createElementFromXml<ColladaEffectTechnique>(this, element);
}
