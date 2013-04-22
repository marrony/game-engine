/*
 * ColladaEffectConstant.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADAEFFECTCONSTANT_H_
#define COLLADAEFFECTCONSTANT_H_

#include "ColladaElement.h"

DECLARE_VISITOR(ColladaEffectConstant);

class ColladaEffectConstant : public ColladaElement {
public:
	static std::string elementType() {
		return "constant";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaElement::loadFromXml(element);
	}

	virtual void accept(engine::Visitor* visitor);
};

#endif /* COLLADAEFFECTCONSTANT_H_ */
