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
	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	virtual void accept(Visitor* visitor);
};

class ColladaEffectLambert : public ColladaEffectConstant {
public:
	static std::string elementType() {
		return "lambert";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaElement::loadFromXml(element);
	}
};

//////////////////////////////////////////////////////////////////

class ColladaEffectBlinn : public ColladaEffectLambert {
public:
	static std::string elementType() {
		return "blinn";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaElement::loadFromXml(element);
	}
};

//////////////////////////////////////////////////////////////////

class ColladaEffectPhong : public ColladaEffectBlinn {
public:
	static std::string elementType() {
		return "phong";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaElement::loadFromXml(element);
	}
};

#endif /* COLLADAEFFECTCONSTANT_H_ */
