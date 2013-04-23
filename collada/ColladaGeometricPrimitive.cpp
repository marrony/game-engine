/*
 * ColladaGeometricPrimitive.cpp
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#include "ColladaGeometricPrimitive.h"
#include "ColladaElement.h"
#include "ColladaInput.h"
#include "ColladaSource.h"
#include "ColladaArray.h"
#include "ColladaUtil.h"

DEFINE_VISITOR(ColladaGeometricPrimitive)

ColladaGeometricPrimitive::~ColladaGeometricPrimitive() {
	ColladaUtil::eraseVector(inputs);
}

void ColladaGeometricPrimitive::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	ColladaUtil::loadAttribute(element, "material", material);
	inputs = ColladaUtil::createElementsFromXml<ColladaInput>(this, element);

	TiXmlElement* xmlPrimitive = element->FirstChildElement("p");
	while(xmlPrimitive) {
		primitive.push_back(std::vector<int>());
		ColladaUtil::loadArray(xmlPrimitive, primitive.back());

		xmlPrimitive = xmlPrimitive->NextSiblingElement("p");
	}
}

ColladaInput* ColladaGeometricPrimitive::findInputWithSemantic(const std::string& semantic) {
	std::vector<ColladaInput*>::iterator input;

	for(size_t i = 0; i < inputs.size(); i++) {
		ColladaInput* input = inputs[i];
		if(input->getSemantic() == semantic)
			return input;
	}

	return 0;
}
