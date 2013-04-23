/*
 * ColladaEffectNewParam.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADANEWPARAM_H_
#define COLLADANEWPARAM_H_

#include "ColladaElement.h"

DECLARE_VISITOR(ColladaNewParam);

class ColladaEffectSampler2D;
class ColladaEffectSurface;
class ColladaEffectSemantic;

class ColladaNewParam : public ColladaElement {
	ColladaEffectSampler2D* sampler2D;
	ColladaEffectSurface* surface;
	ColladaEffectSemantic* semantic;
public:
	virtual ~ColladaNewParam();

	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	virtual void accept(Visitor* visitor);

	ColladaEffectSampler2D* getSampler2D() const { return sampler2D; }
	ColladaEffectSurface* getSurface() const { return surface; }
	ColladaEffectSemantic* getSemantic() const { return semantic; }
};

#endif /* COLLADANEWPARAM_H_ */
