/*
 * ColladaEffectNewParam.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADANEWPARAM_H_
#define COLLADANEWPARAM_H_

#include "ColladaElement.h"
#include "ColladaEffectSampler2D.h"
#include "ColladaEffectSurface.h"
#include "ColladaEffectSemantic.h"

DECLARE_VISITOR(ColladaNewParam);

class ColladaNewParam : public ColladaElement {
	ColladaEffectSampler2D* sampler2D;
	ColladaEffectSurface* surface;
	ColladaEffectSemantic* semantic;
public:
	virtual ~ColladaNewParam() {
		delete sampler2D;
		delete surface;
		delete semantic;
	}

	static std::string elementType() {
		return "newparam";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaElement::loadFromXml(element);

		surface = createElementFromXml<ColladaEffectSurface>(element);
		sampler2D = createElementFromXml<ColladaEffectSampler2D>(element);
		semantic = createElementFromXml<ColladaEffectSemantic>(element);
	}

	virtual void accept(engine::Visitor* visitor);

	ColladaEffectSampler2D* getSampler2D() const { return sampler2D; }
	ColladaEffectSurface* getSurface() const { return surface; }
	ColladaEffectSemantic* getSemantic() const { return semantic; }
};

#endif /* COLLADANEWPARAM_H_ */
