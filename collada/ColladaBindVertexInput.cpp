/*
 * ColladaBindVertexInput.cpp
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#include "ColladaBindVertexInput.h"
#include "ColladaUtil.h"

DEFINE_VISITOR(ColladaBindVertexInput)

std::string ColladaBindVertexInput::elementType() {
	return "bind_vertex_input";
}

void ColladaBindVertexInput::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	ColladaUtil::loadAttribute(element, "semantic", semantic);
	ColladaUtil::loadAttribute(element, "input_semantic", inputSemantic);
	ColladaUtil::loadAttribute(element, "input_set", inputSet);
}
