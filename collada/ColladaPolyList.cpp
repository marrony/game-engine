/*
 * ColladaPolyList.cpp
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#include "ColladaPolyList.h"
#include "ColladaUtil.h"

DEFINE_VISITOR(ColladaPolyList)

std::string ColladaPolyList::elementType() {
	return "polylist";
}

void ColladaPolyList::loadFromXml(TiXmlElement* element) {
	ColladaGeometricPrimitive::loadFromXml(element);

	ColladaUtil::loadArray(element->FirstChildElement("vcount"), vcount);
}
