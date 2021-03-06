/*
 * ColladaInput.cpp
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#include "ColladaInput.h"
#include "ColladaVertices.h"
#include "ColladaSource.h"
#include "ColladaUtil.h"

DEFINE_VISITOR(ColladaInput)

std::string ColladaInput::elementType() {
	return "input";
}

void ColladaInput::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	ColladaUtil::loadAttribute(element, "semantic", semantic);
	ColladaUtil::loadAttribute(element, "source", source);
	ColladaUtil::loadAttribute(element, "offset", offset);
	ColladaUtil::loadAttribute(element, "set", set);
}

const std::string& ColladaInput::getSemantic() {
	return semantic;
}

ColladaSource* ColladaInput::getSourceElement() {
	ColladaElement* element = findElement(source);

	if(ColladaVertices* vertices = dynamic_cast<ColladaVertices*>(element)) {
		ColladaInput* input = vertices->findInputWithSemantic("POSITION");
		element = findElement(input->source);
	}

	return dynamic_cast<ColladaSource*>(element);
}

int ColladaInput::getOffset() {
	std::stringstream ss(offset);

	int value;
	if(ss >> value)
		return value;

	return -1;
}

int ColladaInput::getSet() {
	std::stringstream ss(set);

	int value;
	if(ss >> value)
		return value;

	return -1;
}
