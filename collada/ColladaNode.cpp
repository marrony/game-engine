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

DEFINE_VISITOR(ColladaNode)

ColladaNode::~ColladaNode() {
	eraseVector(nodes);
	eraseVector(instanceGeometries);
	eraseVector(instanceControllers);
	eraseVector(instanceNodes);
	eraseVector(transformations);
}

std::string ColladaNode::elementType() {
	return "node";
}

void ColladaNode::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	nodes = createElementsFromXml<ColladaNode>(element);
	instanceGeometries = createElementsFromXml<ColladaInstanceGeometry>(element);
	instanceControllers = createElementsFromXml<ColladaInstanceController>(element);
	instanceNodes = createElementsFromXml<ColladaInstanceNode>(element);

	TiXmlElement* child = element->FirstChildElement();
	while(child) {
		const char* elementType = child->Value();

		if(ColladaMatrix::elementType() == elementType) {
			transformations.push_back(createElementFromXml2<ColladaMatrix>(child));
		} else if(ColladaRotate::elementType() == elementType) {
			transformations.push_back(createElementFromXml2<ColladaRotate>(child));
		} else if(ColladaScale::elementType() == elementType) {
			transformations.push_back(createElementFromXml2<ColladaScale>(child));
		} else if(ColladaSkew::elementType() == elementType) {
			transformations.push_back(createElementFromXml2<ColladaSkew>(child));
		} else if(ColladaTranslate::elementType() == elementType) {
			transformations.push_back(createElementFromXml2<ColladaTranslate>(child));
		}

		child = child->NextSiblingElement();
	}
}
