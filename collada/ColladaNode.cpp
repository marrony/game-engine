/*
 * ColladaNode.cpp
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#include "ColladaNode.h"
#include "ColladaNode.h"
#include "ColladaInstanceGeometry.h"
#include "ColladaInstanceController.h"
#include "ColladaInstanceNode.h"
#include "ColladaMatrix.h"
#include "ColladaUtil.h"

DEFINE_VISITOR(ColladaNode)

ColladaNode::~ColladaNode() {
	ColladaUtil::eraseVector(nodes);
	ColladaUtil::eraseVector(instanceGeometries);
	ColladaUtil::eraseVector(instanceControllers);
	ColladaUtil::eraseVector(instanceNodes);
	ColladaUtil::eraseVector(transformations);
}

std::string ColladaNode::elementType() {
	return "node";
}

void ColladaNode::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	nodes = ColladaUtil::createElementsFromXml<ColladaNode>(this, element);
	instanceGeometries = ColladaUtil::createElementsFromXml<ColladaInstanceGeometry>(this, element);
	instanceControllers = ColladaUtil::createElementsFromXml<ColladaInstanceController>(this, element);
	instanceNodes = ColladaUtil::createElementsFromXml<ColladaInstanceNode>(this, element);

	TiXmlElement* child = element->FirstChildElement();
	while(child) {
		const char* elementType = child->Value();

		if(ColladaMatrix::elementType() == elementType) {
			transformations.push_back(ColladaUtil::createElementFromXml2<ColladaMatrix>(this, child));
		} else if(ColladaRotate::elementType() == elementType) {
			transformations.push_back(ColladaUtil::createElementFromXml2<ColladaRotate>(this, child));
		} else if(ColladaScale::elementType() == elementType) {
			transformations.push_back(ColladaUtil::createElementFromXml2<ColladaScale>(this, child));
		} else if(ColladaSkew::elementType() == elementType) {
			transformations.push_back(ColladaUtil::createElementFromXml2<ColladaSkew>(this, child));
		} else if(ColladaTranslate::elementType() == elementType) {
			transformations.push_back(ColladaUtil::createElementFromXml2<ColladaTranslate>(this, child));
		}

		child = child->NextSiblingElement();
	}
}
