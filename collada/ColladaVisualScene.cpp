/*
 * ColladaVisualScene.cpp
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#include "ColladaVisualScene.h"
#include "ColladaNode.h"
#include "ColladaUtil.h"

DEFINE_VISITOR(ColladaVisualScene)

ColladaVisualScene::~ColladaVisualScene() {
	ColladaUtil::eraseVector(nodes);
}

std::string ColladaVisualScene::elementType() {
	return "visual_scene";
}

void ColladaVisualScene::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	nodes = ColladaUtil::createElementsFromXml<ColladaNode>(this, element);
}
