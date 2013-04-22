/*
 * ColladaScene.cpp
 *
 *  Created on: 21/05/2012
 *      Author: marrony
 */

#include "ColladaScene.h"

#include "tinyxml.h"

DEFINE_VISITOR(ColladaInstanceVisualScene)

void ColladaInstanceVisualScene::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	loadAttribute(element, "sid", sid);
	loadAttribute(element, "url", url);
}

DEFINE_VISITOR(ColladaScene)

void ColladaScene::loadFromXml(class TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	instanceVisualScene = createElementFromXml<ColladaInstanceVisualScene>(element);
}
