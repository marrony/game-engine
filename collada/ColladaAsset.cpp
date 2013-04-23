/*
 * ColladaAsset.cpp
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#include "ColladaAsset.h"

DEFINE_VISITOR(ColladaAsset)

std::string ColladaAsset::elementType() {
	return "asset";
}

void ColladaAsset::loadFromXml(class TiXmlElement* element) {
}
