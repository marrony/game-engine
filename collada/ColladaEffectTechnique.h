/*
 * ColladaEffectTechnique.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADAEFFECTTECHNIQUE_H_
#define COLLADAEFFECTTECHNIQUE_H_

#include "ColladaElement.h"
#include "ColladaEffectConstant.h"

DECLARE_VISITOR(ColladaEffectTechnique);

class ColladaEffectTechnique : public ColladaElement {
	std::string sid;
	ColladaEffectConstant* shader;
public:
	~ColladaEffectTechnique() {
		delete shader;
	}

	static std::string elementType() {
		return "technique";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaElement::loadFromXml(element);

		loadAttribute(element, "sid", sid);

		shader = createElementFromXml<ColladaEffectConstant>(element);

//		if(!shader) {
//			shader = createElementFromXml<ColladaEffectLambert>(element);
//		}
//
//		if(!shader) {
//			shader = createElementFromXml<ColladaEffectBlinn>(element);
//		}
//
//		if(!shader) {
//			shader = createElementFromXml<ColladaEffectPhong>(element);
//		}
	}

	virtual void accept(engine::Visitor* visitor);
};

#endif /* COLLADAEFFECTTECHNIQUE_H_ */
