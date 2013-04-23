/*
 * ColladaEffectTechnique.cpp
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#include "ColladaEffectTechnique.h"
#include "ColladaEffectConstant.h"
#include "ColladaUtil.h"

DEFINE_VISITOR(ColladaEffectTechnique)

ColladaEffectTechnique::~ColladaEffectTechnique() {
	delete shader;
}

std::string ColladaEffectTechnique::elementType() {
	return "technique";
}

void ColladaEffectTechnique::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	ColladaUtil::loadAttribute(element, "sid", sid);

	shader = ColladaUtil::createElementFromXml<ColladaEffectConstant>(this, element);

//		if(!shader) {
//			shader = createElementFromXml<ColladaEffectLambert>(element);
//		}
//
//		if(!shader) {
//			shader = createElementFromXml<ColladaEffectBlinn>(element);
//		}
//
//		if(!shader) {
//			shader = createElementFromXml<ColladaEffectPhong>(element);
//		}
}
