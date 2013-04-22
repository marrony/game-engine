/*
 * ColladaPrimitive.h
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#ifndef COLLADAPRIMITIVE_H_
#define COLLADAPRIMITIVE_H_

#include "ColladaElement.h"

DECLARE_VISITOR(ColladaPrimitive);

class ColladaPrimitive : public ColladaElement {
	std::vector<int> indices;
public:
	static std::string elementType() {
		return "p";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaElement::loadFromXml(element);

		std::string value = element->GetText();
		std::stringstream stream(value);
		std::istream_iterator<int> beginOfStream(stream);
		std::istream_iterator<int> endOfStream;

		std::copy(beginOfStream, endOfStream, std::back_inserter(indices));
	}

	virtual void accept(engine::Visitor* visitor);

	std::vector<int> getIndices() {
		return indices;
	}
};

#endif /* COLLADAPRIMITIVE_H_ */
