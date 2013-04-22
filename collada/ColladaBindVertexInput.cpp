/*
 * ColladaBindVertexInput.cpp
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#include "ColladaBindVertexInput.h"

#include "tinyxml.h"

DEFINE_VISITOR(ColladaBindVertexInput)

std::string ColladaBindVertexInput::elementType() {
	return "bind_vertex_input";
}

void ColladaBindVertexInput::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	loadAttribute(element, "semantic", semantic);
	loadAttribute(element, "input_semantic", inputSemantic);
	loadAttribute(element, "input_set", inputSet);
}
