/*
 * ColladaCamera.cpp
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#include "ColladaCamera.h"

#include "tinyxml.h"

DEFINE_VISITOR(ColladaCamera)

std::string ColladaCamera::elementType() {
	return "camera";
}

void ColladaCamera::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	//TODO parse camera
}
