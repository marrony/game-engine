/*
 * ColladaInstanceController.cpp
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#include "ColladaInstanceController.h"
#include "ColladaSkeleton.h"
#include "ColladaBindMaterial.h"
#include "ColladaUtil.h"

DEFINE_VISITOR(ColladaInstanceController)

ColladaInstanceController::~ColladaInstanceController() {
	ColladaUtil::eraseVector(skeletons);
	delete bindMaterial;
}

std::string ColladaInstanceController::elementType() {
	return "instance_controller";
}

void ColladaInstanceController::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	skeletons = ColladaUtil::createElementsFromXml<ColladaSkeleton>(this, element);
	bindMaterial = ColladaUtil::createElementFromXml<ColladaBindMaterial>(this, element);
}
