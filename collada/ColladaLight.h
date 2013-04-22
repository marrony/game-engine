/*
 * ColladaLight.h
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#ifndef COLLADALIGHT_H_
#define COLLADALIGHT_H_

#include "ColladaElement.h"

DECLARE_VISITOR(ColladaLight);

class ColladaLight : public ColladaElement {
public:
	static std::string elementType() {
		return "light";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaElement::loadFromXml(element);

		//TODO parse light
	}

	virtual void accept(engine::Visitor* visitor);
};

#endif /* COLLADALIGHT_H_ */
