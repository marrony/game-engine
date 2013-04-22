/*
 * ColladaEffectSampler2D.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADAEFFECTSAMPLER2D_H_
#define COLLADAEFFECTSAMPLER2D_H_

#include "ColladaElement.h"

DECLARE_VISITOR(ColladaEffectSampler2D);

class ColladaEffectSampler2D : public ColladaElement {
public:
	static std::string elementType() {
		return "sampler2D";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaElement::loadFromXml(element);
	}

	virtual void accept(engine::Visitor* visitor);
};

#endif /* COLLADAEFFECTSAMPLER2D_H_ */
