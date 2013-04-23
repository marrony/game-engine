/*
 * ColladaVertices.cpp
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#include "ColladaVertices.h"
#include "ColladaInput.h"
#include "ColladaUtil.h"

DEFINE_VISITOR(ColladaVertices)

ColladaVertices::~ColladaVertices() {
	ColladaUtil::eraseVector(inputs);
}

std::string ColladaVertices::elementType() {
	return "vertices";
}

void ColladaVertices::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	inputs = ColladaUtil::createElementsFromXml<ColladaInput>(this, element);
	//TODO deve existir um input com semmantic=POSITION
}

ColladaInput* ColladaVertices::findInputWithSemantic(const std::string& semantic) {
	std::vector<ColladaInput*>::iterator input;

	for(input = inputs.begin(); input != inputs.end(); input++) {
		if((*input)->getSemantic() == semantic) {
			return *input;
		}
	}

	return 0;
}
