/*
 * ColladaInstanceNode.cpp
 *
 *  Created on: 22/05/2012
 *      Author: marrony.neris
 */

#include "ColladaInstanceNode.h"
#include "ColladaUtil.h"

DEFINE_VISITOR(ColladaInstanceNode)

std::string ColladaInstanceNode::elementType() {
	return "instance_node";
}

void ColladaInstanceNode::loadFromXml(TiXmlElement* element) {
	ColladaUtil::loadAttribute(element, "url", url);
}
