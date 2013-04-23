/*
 * ColladaImage.cpp
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#include "ColladaImage.h"

DEFINE_VISITOR(ColladaImage)

std::string ColladaImage::elementType() {
	return "image";
}

void ColladaImage::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	TiXmlElement* data = element->FirstChildElement("data");
	if(data != 0) {
		//throw Exception("not supported");
	}

	TiXmlElement* init_from = element->FirstChildElement("init_from");
	if(init_from != 0) {
		path = init_from->GetText();
	}
}
