/*
 * ColladaExtra.cpp
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#include "ColladaExtra.h"

DEFINE_VISITOR(ColladaExtra)

std::string ColladaExtra::elementType() {
	return "extra";
}

void ColladaExtra::loadFromXml(TiXmlElement* element) {
}
