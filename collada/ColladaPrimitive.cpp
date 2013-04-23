/*
 * ColladaPrimitive.cpp
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#include "ColladaPrimitive.h"

DEFINE_VISITOR(ColladaPrimitive)

std::string ColladaPrimitive::elementType() {
	return "p";
}

void ColladaPrimitive::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	std::string value = element->GetText();
	std::stringstream stream(value);
	std::istream_iterator<int> beginOfStream(stream);
	std::istream_iterator<int> endOfStream;

	std::copy(beginOfStream, endOfStream, std::back_inserter(indices));
}

