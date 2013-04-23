/*
 * ColladaScene.cpp
 *
 *  Created on: 21/05/2012
 *      Author: marrony
 */

#include "ColladaScene.h"
#include "ColladaUtil.h"

DEFINE_VISITOR(ColladaInstanceVisualScene)

std::string ColladaInstanceVisualScene::elementType() {
	return "instance_visual_scene";
}

void ColladaInstanceVisualScene::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	ColladaUtil::loadAttribute(element, "sid", sid);
	ColladaUtil::loadAttribute(element, "url", url);
}

DEFINE_VISITOR(ColladaScene)

ColladaScene::~ColladaScene() {
	delete instanceVisualScene;
}

std::string ColladaScene::elementType() {
	return "scene";
}

void ColladaScene::loadFromXml(class TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	instanceVisualScene = ColladaUtil::createElementFromXml<ColladaInstanceVisualScene>(this, element);
}
