/*
 * ColladaSkeleton.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADASKELETON_H_
#define COLLADASKELETON_H_

#include "ColladaElement.h"

DECLARE_VISITOR(ColladaSkeleton);

class ColladaSkeleton : public ColladaElement {
	std::string uri;
public:
	~ColladaSkeleton() {
	}

	static std::string elementType() {
		return "skeleton";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaElement::loadFromXml(element);

		uri = element->GetText();
	}

	virtual void accept(engine::Visitor* visitor);
};

#endif /* COLLADASKELETON_H_ */
