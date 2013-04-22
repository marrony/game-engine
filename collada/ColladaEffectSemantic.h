/*
 * ColladaEffectSemantic.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADAEFFECTSEMANTIC_H_
#define COLLADAEFFECTSEMANTIC_H_

#include "ColladaElement.h"

DECLARE_VISITOR(ColladaEffectSemantic);

class ColladaEffectSemantic : public ColladaElement {
	std::string semantic;
public:
	static std::string elementType() {
		return "semantic";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaElement::loadFromXml(element);

		semantic = element->GetText();
	}

	virtual void accept(engine::Visitor* visitor);
};

#endif /* COLLADAEFFECTSEMANTIC_H_ */
